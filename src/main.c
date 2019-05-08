//
// Created by amin on 12/18/18.
//


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <time.h>
#include "physics.h"
#include "structs.h"
#include "view.h"
#include "init.h"
#include "logic.h"
#include "UI.h"
#include "effects.h"
#include "power-ups.h"



int main() {

    /// SDL startings
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Alter Tank", SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED, 1325, 1000, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    ///initializing
    Map *map = malloc(sizeof(Map));
    initialize(map, window, renderer);
    const float FPS = 4/sqrt(map->players);
    bool finished = false;


    if(!start_game(map))
        return 0;



    while (handle_event(map) && !finished) {

        ///seting renderer
        int start_ticks = SDL_GetTicks();
        SDL_SetRenderDrawColor(map->renderer, 206, 239, 255, 255);
        SDL_RenderClear(map->renderer);

        ///handling keys
        handle_keys(map);
        if(map->ai_mode && map->tanks[1]->is_alive)
            go_ai(map);



        ///showings
        show_walls(map, renderer);
        for(int i = 0 ; i<map->players ; i++)
            if(map->tanks[i]->is_alive)
                show_tank(map->tanks[i], renderer);
        show_scores(map , renderer);
        if(map->bullets != NULL)
            show_bullet(map, renderer);
        for(int i = 0 ;i<map->power_ups_number ; i++)
            show_power_up(map, map->power_ups[i]);


        ///End time
        finished = check_end(map, &renderer, window);

        ///Bullets and Power-Ups
        refresh_bullets(map);
        refresh_powerups(map);

        ///present render
        SDL_RenderPresent(renderer);
        while (SDL_GetTicks() - start_ticks < FPS);
    }


    ///Save game
    FILE *f = fopen("lastGame.txt", "w");
    if(map->ai_mode && map->players == 2)
        map->players--;
    fprintf(f, "%d %d %d\n%d %d %d %d",map->round-1, map->players , map->ai_mode,
           map->tanks[0]->score, map->tanks[1]->score, map->tanks[2]->score, map->tanks[3]->score);
    fclose(f);

    /// SDL endings
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


