//
// Created by amin on 12/18/18.
//



#ifndef PROJECT_PHYSICS_H
#define PROJECT_PHYSICS_H


#include "structs.h"

int handle_event(Map *map);
void handle_keys(Map *map);
void add_bullet(Bullet *b, Bullet_Node **bullets);
void update_corners(Tank *t);
void go_ai(Map *map);
void refresh_bullets(Map *map) ;

#endif //PROJECT_PHYSICS_H


