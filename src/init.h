//
// Created by amin on 12/24/18.
//

#ifndef PROJECT_INIT_H
#define PROJECT_INIT_H

#include "structs.h"

static bool button(button_t *btn);
int starting_UI(Map *map);
Bullet* init_bullet(Tank *t, double ratio);
void init_tank(Map *map, int k, bool *** ocupied);
bool start_game(Map *map);
void generate_walls(Cell ***cells, int a, int b);
int load_last_game(Map *map);
void create_tanks(Map *map) ;
void initialize(Map *map, SDL_Window *window, SDL_Renderer *renderer);

#endif //PROJECT_INIT_H
