//
// Created by amin on 12/25/18.
//

#include <SDL2_gfxPrimitives.h>
#include <zconf.h>
#include "logic.h"





#include "structs.h"
#include "physics.h"
#include "init.h"
#include "effects.h"
#include "view.h"


#define black 0, 0, 0, 255

double min(double a, double b) {
    if(a<b)
        return a;
    return b;
}


double max(double a, double b){
    if(a>b)
        return a;
    return b;
}

bool is_lefthand(Point p, Line l) {
    return ((l.p[1].x - l.p[0].x)*(p.y-l.p[0].y) - (l.p[1].y-l.p[0].y)*(p.x-l.p[0].x) < 0);
}

bool is_inside(Point p, Tank *t) {
    update_corners(t);
    for(int i = 0 ; i<5 ; i++){
        Line l;
        l.p[0].x = t->corners[i].x;
        l.p[0].y = t->corners[i].y;
        l.p[1].x = t->corners[(i+1)%5].x;
        l.p[1].y = t->corners[(i+1)%5].y;
        if(is_lefthand(p, l))
            return false;
    }
    return true;
}

bool collide(Wall *wall, double x, double y, double radius, Tank *t) {
    Point p1,p2;
    p1.x = wall->pos[0];
    p1.y = wall->pos[1];
    p2.x = wall->pos[2];
    p2.y = wall->pos[3];
    if (wall->dir == HORIZONTAL) {
        if (x > min(wall->pos[0], wall->pos[2]) && x < max(wall->pos[0], wall->pos[2]) && abs(y - wall->pos[3]) < radius)
            return 1;
        if(is_inside(p1, t) || is_inside(p2, t))
            return 1;
    } else {
        if (y > min(wall->pos[1], wall->pos[3]) && y < max(wall->pos[1], wall->pos[3]) && abs(x - wall->pos[0]) < radius)
            return 1;
        if(is_inside(p1, t) || is_inside(p2, t))
            return 1;
    }

    return 0;
}

bool can_go(Map *map, int player_number, int dir) {
    double radius = 0.01;
    for(int i = 0 ; i<map->players ;i++)
        update_corners(map->tanks[i]);
    for(int i = 0 ; i<map->count_of_walls ; i++){
        for(int j = dir ; j<3+2*(dir&1) ;j++) {
            Point p;
            p.x = map->tanks[player_number]->corners[j].x;
            p.y = map->tanks[player_number]->corners[j].y;
            if (collide(map->walls[i], p.x, p.y, radius, map->tanks[player_number]))
                return 0;
            for (int k = 0; k < map->players; k++)
                if (k != player_number && map->tanks[k]->is_alive && is_inside(p, map->tanks[k]))
                    return 0;
        }
    }
    return 1;
}

bool can_turn(Map *map , int player_number) {
    update_corners(map->tanks[player_number]);
    double x = map->tanks[player_number]->x, y = map->tanks[player_number]->y, a = map->tanks[player_number]->angle, radius = 0.01;
    for(int i = 0 ; i<map->count_of_walls ; i++){
        for(int j = 0 ; j<5 ;j++) {
            Point p;
            p.x = map->tanks[player_number]->corners[j].x;
            p.y = map->tanks[player_number]->corners[j].y;
            if (collide(map->walls[i], p.x, p.y, radius, map->tanks[player_number]))
                return 0;
            for (int k = 0; k < map->players; k++)
                if (k != player_number && map->tanks[k]->is_alive && is_inside(p, map->tanks[k]))
                    return 0;
        }
    }
    return 1;
}

int meet_wall(Map *map, Bullet b) {
    for(int i = 0 ; i<map->count_of_walls ; i++){
        if(map->walls[i]->dir == HORIZONTAL) {
            double minpos = min(map->walls[i]->pos[0], map->walls[i]->pos[2]), maxpos = max(map->walls[i]->pos[0], map->walls[i]->pos[2]);
            if(abs(b.y - map->walls[i]->pos[1]) < b.radius && ((abs(b.x - minpos) < b.radius && b.x < minpos )
                                                            || (abs(b.x - maxpos) < b.radius && b.x > maxpos )))
                return 2;
            if(b.x >  minpos && b.x < maxpos && abs(b.y-map->walls[i]->pos[3])<b.radius)
                return 1;

        } else {
            double minpos = min(map->walls[i]->pos[1], map->walls[i]->pos[3]), maxpos = max(map->walls[i]->pos[1], map->walls[i]->pos[3]);
            if(abs(b.x - map->walls[i]->pos[0]) < b.radius && ((abs(b.y - minpos) < b.radius && b.y < minpos )
                                                               || (abs(b.y - maxpos) < b.radius && b.y > maxpos )))
                return 1;
            if (b.y > minpos && b.y < maxpos && abs(b.x - map->walls[i]->pos[0]) < b.radius)
                return 2;

        }
    }
    return  0;
}

int check_end(Map *map, SDL_Renderer **renderer, SDL_Window *window) {
    if(map->end_time != 0){
        if(map->end_time > 0)
            map->end_time--;
        return 0;
    }
    int winner = -1;
    for(int i = 0 ; i<map->players; i++)
        if(map->tanks[i]->is_alive){
            winner = i;
            map->tanks[i]->score++;
            if(map->max_point < map->tanks[i]->score)
                map->max_point  = map->tanks[i]->score;
        }


    double midx = 1325/2 ,midy =500;

    bool finished = map->max_point == map->target;
    if(winner > -1){
        for (int i = 0; i < 2; ++i) {
            SDL_SetRenderDrawColor(*renderer, 163, 255, 201, 255);
            if(finished)
                SDL_SetRenderDrawColor(*renderer, 0, 255, 0, 255);
            SDL_RenderClear(*renderer);
            Tank *tmp_tank = map->tanks[winner];
            tmp_tank->angle = M_PI/2;
            tmp_tank->lenght = 300;
            tmp_tank->width = 250;
            tmp_tank->barrel_lenght = 200;
            tmp_tank->barrel_thickness = 40;
            tmp_tank-> y = midy;
            tmp_tank->x = midx-20;
            show_tank(tmp_tank, *renderer);
            if(map->ai_mode && winner == 1)
                stringRGBA(*renderer, midx+tmp_tank->width/2, midy-10, "I won.", black);
            else
                stringRGBA(*renderer, midx+tmp_tank->width/2, midy-10, "You won.", black);
            char s[20] ;
            sprintf(s,"%d",tmp_tank->score+i-1);
            char ss[30] = "Score: ";
            strcat(ss, s);
            stringRGBA(*renderer, midx+tmp_tank->width/2, midy + 10, ss, black);

            if( i == 1 && finished){
                if(map->ai_mode && winner == 1)
                    stringRGBA(*renderer, midx-110, midy + tmp_tank->lenght/2 +150, "I am the final winner !!!", black);
                else
                    stringRGBA(*renderer, midx-180, midy + tmp_tank->lenght/2 +150, "Congratulations,You are the final winner !!!", black);
            }
            SDL_RenderPresent(*renderer);
            SDL_Delay(1000+500*i + i * finished * 5000);
        }
    } else {
        SDL_SetRenderDrawColor(*renderer, 255, 175, 175, 255);
        SDL_RenderClear(*renderer);
        stringRGBA(*renderer, midx-40, midy, "Nobody won.", black);
        SDL_RenderPresent(*renderer);
        SDL_Delay(1500);
    }

    if(finished)
        return 1;
    if(map->ai_mode)
        map->players = 1;
    start_game(map);
    return 0;
}

bool meet_tank(Map *map, Bullet b){
    Point p;
    p.x = b.x;
    p.y = b.y;
    for(int i = 0 ; i<map->players ; i++){
        Tank *t = map->tanks[i];
        if(!t->is_alive) continue;
        update_corners(t);
        if(is_inside(p, t)){
            map->tanks[i]->is_alive = false;
            play_sound_effect();
            /// TODO play sound without delay
            int c = 0 ;
            for(int i= 0 ; i<map->players ; i++){
                if(map->tanks[i]->is_alive)
                    c++;
            }
            if(c  == 1)
                map->end_time = 1000;
            return 1;
        }
    }
    return 0;
}