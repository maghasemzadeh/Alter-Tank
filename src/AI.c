//
// Created by amin on 1/5/19.
//

#include "AI.h"
#include "logic.h"

bool is_near_to_wall(Map *map) {
    double k = 20;
    for(int i = 0 ; i<k; i++){
        map->tanks[1]->x += cos(-map->tanks[1]->angle);
        map->tanks[1]->y += sin(-map->tanks[1]->angle);
        if(!can_go(map, 1, 0)){
            map->tanks[1]->x -= (i+1)*cos(-map->tanks[1]->angle);
            map->tanks[1]->y -= (i+1)*sin(-map->tanks[1]->angle);
            return 1;
        }
    }
    map->tanks[1]->x -= k * cos(-map->tanks[1]->angle);
    map->tanks[1]->y -= k*sin(-map->tanks[1]->angle);
    return 0;
}


bool is_dangerous(Map *map) {
    double m = (map->tanks[1]->y - map->tanks[0]->y)/(map->tanks[1]->x - map->tanks[0]->x);
    double b = -m*map->tanks[1]->x + map->tanks[1]->y;
    for (int i = 0; i < map->count_of_walls; ++i) {
        double x , y;
        if(map->walls[i]->dir == HORIZONTAL){
            m = 1/m;
            b *= -m;
            y = map->walls[i]->pos[1];
            x = m*y + b;
            if(x<max(map->walls[i]->pos[0], map->walls[i]->pos[2]) && x>min(map->walls[i]->pos[0], map->walls[i]->pos[2])
                && x>min(map->tanks[0]->x, map->tanks[1]->x) && x<max(map->tanks[0]->x, map->tanks[1]->x)
                && y>min(map->tanks[0]->y, map->tanks[1]->y) && y<max(map->tanks[0]->y, map->tanks[1]->y))
            return 0;
        }else{
            x = map->walls[i]->pos[0];
            y = m*x + b;
            if(y<max(map->walls[i]->pos[1], map->walls[i]->pos[3]) && y>min(map->walls[i]->pos[1], map->walls[i]->pos[3])
               && x>min(map->tanks[0]->x, map->tanks[1]->x) && x<max(map->tanks[0]->x, map->tanks[1]->x)
               && y>min(map->tanks[0]->y, map->tanks[1]->y) && y<max(map->tanks[0]->y, map->tanks[1]->y))
            return 0;
        }
    }
    return 1;
}

void move(Map *map) {
    if (is_near_to_wall(map)) {
        map->tanks[1]->x -= cos(-map->tanks[1]->angle);
        map->tanks[1]->y -= sin(-map->tanks[1]->angle);
        map->tanks[1]->angle += 0.025;
        if (!can_turn(map, 1))
            map->tanks[1]->angle -= 0.05;
        if (!can_go(map, 1, 3)) {
            map->tanks[1]->x += cos(-map->tanks[1]->angle);
            map->tanks[1]->y += sin(-map->tanks[1]->angle);
        }
    }
    else {
        map->tanks[1]->x += cos(-map->tanks[1]->angle);
        map->tanks[1]->y += sin(-map->tanks[1]->angle);
        if (!can_go(map, 1, 0)) {
            map->tanks[1]->x -= cos(-map->tanks[1]->angle);
            map->tanks[1]->y -= sin(-map->tanks[1]->angle);
        }
    }
}

void go_ai(Map *map) {
    if(is_dangerous(map))
        move(map);
}

