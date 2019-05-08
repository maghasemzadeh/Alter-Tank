//
// Created by amin on 12/18/18.
//

#include "structs.h"

#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H

void show_tank(Tank *t, SDL_Renderer *pRenderer);
void show_walls(Map *map, struct SDL_Renderer * renderer);
void updlode_walls(Map *map);
void show_bullet(Map *map, SDL_Renderer *renderer);
void show_scores(Map *map, SDL_Renderer *renderer);
void walls_to_file(Cell **cells, int n, int m);
int to_int(char buff[5]);
void *to_letter(char *result, int i);
void show_button(SDL_Renderer *r, button_t *btn, Map *map, bool error);
void show_power_up(Map *map, power_up *param);

#endif //PROJECT_VIEW_H
