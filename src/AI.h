//
// Created by amin on 1/5/19.
//

#ifndef PROJECT_AI_H
#define PROJECT_AI_H

#include "structs.h"

bool is_dangerous(Map *map);
void go_ai(Map *map);
void move(Map *map);
bool is_near_to_wall(Map *map);

#endif //PROJECT_AI_H
