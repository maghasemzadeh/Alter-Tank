//
// Created by amin on 12/18/18.
//

#include <SDL_events.h>
#include <time.h>
#include <zconf.h>
#include "physics.h"
#include "logic.h"
#include "init.h"
#include "UI.h"
#include "logic.h"
#include "power-ups.h"
#include "effects.h"


void refresh_bullets(Map *map) {
    for(int i = 0 ; i<map->players ; i++){
        if(!map->tanks[i]->refresh_time){
            map->tanks[i]->bullet++;
            map->tanks[i]->refresh_time = 3000;
        }
        if(map->tanks[i]->bullet < 5)
            map->tanks[i]->refresh_time--;
    }
}

void fire(Tank *t, Bullet_Node **bullets, double ratio) {
    if (!t->bullet) {
        printf("%d bullet available.\n", t->bullet);
        return;
    }
    t->bullet--;
    add_bullet(init_bullet(t , ratio), bullets);
}


void handle_keys(Map *map) {
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);


    for (int i = 0; i < map->players - map->ai_mode; i++) {
        if(map->tanks[i]->is_alive){
            for (int j = 0; j < 2; ++j) {
                if (keyboard[map->tanks[i]->keys[j]]) {
                    map->tanks[i]->angle += ((j & 1)-0.5) * 4.0 * (0.5 - keyboard[map->tanks[i]->keys[3]]) * 0.025 ;
                    if (!can_turn(map , i)){
                        map->tanks[i]->angle -= ((j & 1) - 0.5) * 4.0 * (0.5 - keyboard[map->tanks[i]->keys[3]]) * 0.025 ;
                    }
                }

            }
            for (int j = 2; j < 4; ++j) {
                if (keyboard[map->tanks[i]->keys[j]]) {
                    map->tanks[i]->x += (0.5-(j & 1)) * 2.0 * cos(-map->tanks[i]->angle);
                    map->tanks[i]->y += (0.5-(j & 1)) * 2.0 * sin(-map->tanks[i]->angle);
                    int tmp = (!(j & 1) ? 0 : 3);
                    if (!can_go(map , i , tmp)) {
                        map->tanks[i]->x -= (0.5-(j & 1)) * 2.0 * cos(-map->tanks[i]->angle);
                        map->tanks[i]->y -= (0.5-(j & 1)) * 2.0 * sin(-map->tanks[i]->angle);

                    }
                }
            }
        }
    }
    for(int i = 0 ; i<map->players; i++){
        for(int k = 0 ; k<map->power_ups_number ; k++){
            Point p;
            p.x = map->power_ups[k]->x;
            p.y = map->power_ups[k]->y;
            if(is_inside(p, map->tanks[i]))
                collect(map, map->tanks[i], k--);

        }
    }
    for(int i = 0 ; i<map->players; i++){
        for(int k = 0 ; k<map->mines_number; k++){
            Point p;
            p.x = map->mines[k]->x;
            p.y = map->mines[k]->y;
            if(map->mines[k]->is_activated && is_inside(p, map->tanks[i])){
                play_sound_effect();
                Mine **tmp = map->mines;
                map->mines = malloc((--map->mines_number)*sizeof(Mine*));
                for(int j = 0 ; j< map->mines_number ; j++){
                    if(j<k)
                        map->mines[j] = tmp[j];
                    else
                        map->mines[j] = tmp[j+1];
                }
                map->tanks[i]->is_alive = false;
                int c = 0 ;
                for(int i= 0 ; i<map->players ; i++){
                    if(map->tanks[i]->is_alive)
                        c++;
                }
                if(c  == 1)
                    map->end_time = 1000;
            }
            else if(!map->mines[k]->is_activated ){
                bool flag = true;
                for(int l = 0 ; l< map->players ; l++){
                    if(is_inside(p, map->tanks[l]) )
                        flag = false;
                }
                map->mines[k]->is_activated = flag;
            }
        }
    }

}

int handle_event(Map *map) {
    SDL_Event *event = malloc(sizeof(SDL_Event));
    while (SDL_PollEvent(event)) {
        if(event->type == SDL_QUIT ||
           (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_CLOSE)) {
            return 0;
        }
        if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE){
            return pause_UI(map);
        }
        if(event->type == SDL_KEYUP ){
            for(int i= 0 ; i<4 ; i++){
                if(map->players>i && event->key.keysym.scancode == map->tanks[i]->keys[4] && map->tanks[i]->is_alive)
                    if(map->tanks[i]->power_up->model){
                        fire_powerup(map, map->tanks[i]);
                        map->tanks[i]->power_up->model = 0;
                    }
                    else{
                        fire(map->tanks[i], &map->bullets, map->ratio);
                    }
            }
        }
    }
    return 1;
}

void add_bullet(Bullet *b, Bullet_Node **bullets) {
    Bullet_Node *new_bull = malloc(sizeof(Bullet_Node));
    new_bull->b = *b;
    if(*bullets != NULL) {
        new_bull->next = *bullets;
    }
    else {
        new_bull->next = NULL;
        *bullets = malloc(sizeof(Bullet_Node));
    }
    *bullets = new_bull;
}

void update_corners(Tank *t){
    double a = t->angle, y = t->y , x = t->x;
    t->corners[0].x = x + (t->lenght / 2) * cos(a) - (t->width / 2) * sin(a);
    t->corners[0].y = y - (t->lenght / 2) * sin(a) - (t->width / 2) * cos(a);
    t->corners[1].x = x + t->barrel_lenght * cos(a);
    t->corners[1].y = y - t->barrel_lenght * sin(a);
    t->corners[2].x =  x + (t->lenght / 2) * cos(a) + (t->width / 2) * sin(a);
    t->corners[2].y = y - (t->lenght / 2) * sin(a) + (t->width / 2) * cos(a);
    t->corners[3].x = x - t->lenght / 2 * cos(a) + t->width / 2 * sin(a);
    t->corners[3].y = y + t->lenght / 2 * sin(a) + t->width / 2 * cos(a);
    t->corners[4].x = x - t->lenght / 2 * cos(a) - t->width / 2 * sin(a);
    t->corners[4].y = y + t->lenght / 2 * sin(a) - t->width / 2 * cos(a);
}



