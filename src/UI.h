//
// Created by amin on 1/22/19.
//

#include <SDL_render.h>
#include <stdbool.h>
#include "structs.h"

#ifndef PROJECT_UI_H
#define PROJECT_UI_H



static void button_process_event(button_t *buttons, int k, const SDL_Event *ev, int *focus, int mode) ;
static bool button(button_t *btn);
int starting_UI(Map *map) ;
int pause_UI(Map *map);
int settings_UI(Map *map);

#endif //PROJECT_UI_H

