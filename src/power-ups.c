//
// Created by amin on 1/31/19.
//

#include <time.h>
#include "power-ups.h"



void refresh_powerups(Map *map) {
    for(int i = 0  ; i<map->mines_number ; i++)
        if(map->mines[i]->life-- < 0){
            for(int j = i ; j<map->mines_number-1 ; j++)
                map->mines[j] = map->mines[j+1];
            map->mines_number--;
        }
    if(map->powerup_time-- > 0)
        return;
    power_up **p = map->power_ups;
    map->power_ups = malloc((++map->power_ups_number) * sizeof(power_up*));
    for(int i = 0 ;i<map->power_ups_number-1 ; i++)
        map->power_ups[i] = p[i];
    power_up *tmp = malloc(sizeof(power_up));
    srand(time(NULL));
    tmp->x = map->ratio*(-0.35+(rand()%((int)map->maxx))) + (1000-map->ratio*(map->maxx-1))/2 + 40;
    tmp->y = map->ratio*(-0.35+(rand()%((int)map->maxy))) + (1000-map->ratio*(map->maxy-1))/2 + 40;
    tmp->model = 1;
    map->power_ups[map->power_ups_number-1] = tmp;
    map->powerup_time = 10000;
}


void collect(Map *map, Tank *t, int k) {
    t->power_up = map->power_ups[k];
    map->power_ups_number--;
    power_up **tmp = map->power_ups;
    map->power_ups = malloc(map->power_ups_number* sizeof(power_up*));
    for(int i= 0 ; i<map->power_ups_number ; i++){
        if(i < k)
            map->power_ups[i] = tmp[i];
        else
            map->power_ups[i] = tmp[i+1];
    }

}


void fire_mine(Map *map, Tank *t) {
    Mine **tmp = map->mines;
    map->mines = malloc((++map->mines_number)*sizeof(Mine*));
    for(int i = 0 ; i<map->mines_number-1 ; i++)
        map->mines[i] = tmp[i];
    Mine *tmp2 = malloc(sizeof(Mine));
    tmp2->is_activated = false;
    tmp2->x = t->x;
    tmp2->y = t->y;
    tmp2->life = 10000;
    map->mines[map->mines_number-1] = tmp2;

}

void fire_powerup(Map *map, Tank *t) {
    switch (t->power_up->model){
        case 1:
            fire_mine(map, t);
            break;
        default:
            break;
    }

}
