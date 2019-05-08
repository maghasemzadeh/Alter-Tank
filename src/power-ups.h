//
// Created by amin on 1/31/19.
//

#ifndef PROJECT_POWER_UPS_H
#define PROJECT_POWER_UPS_H


#include "structs.h"

void refresh_powerups(Map *map) ;
void collect(Map *map, Tank *t, int k);
void fire_powerup(Map *map, Tank *ptr);

#endif //PROJECT_POWER_UPS_H
