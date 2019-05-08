//
// Created by amin on 1/22/19.
//

#include <SDL2_gfxPrimitives.h>
#include "UI.h"
#include "init.h"
#include "structs.h"
#include "view.h"
#include <stdlib.h>
#include <stdio.h>


#define black 0,0 ,0, 255

int deltax[] = { +90  , -90 ,  0   ,  0  , 0 }
   ,deltay[] = {  0   ,  0  , -90  , +90 , 0 };



static void button_process_event(button_t *buttons, int k, const SDL_Event *ev, int *focus, int mode) {
    // react on mouse click within button rectangle by setting 'pressed'
    if(ev->type == SDL_MOUSEMOTION ){
        if(ev->button.x >= buttons[k].draw_rect.x &&
           ev->button.x <= (buttons[k].draw_rect.x + buttons[k].draw_rect.w) &&
           ev->button.y >= buttons[k].draw_rect.y &&
           ev->button.y <= (buttons[k].draw_rect.y + buttons[k].draw_rect.h) &&
           !buttons[k].focused) {

            buttons[k].colour.b -= 30;
            buttons[k].colour.g -= 30;
            buttons[k].colour.r -= 30;
            buttons[k].focused = true;
            *focus = k;
        }
        else if(!(ev->button.x >= buttons[k].draw_rect.x &&
                  ev->button.x <= (buttons[k].draw_rect.x + buttons[k].draw_rect.w) &&
                  ev->button.y >= buttons[k].draw_rect.y &&
                  ev->button.y <= (buttons[k].draw_rect.y + buttons[k].draw_rect.h)) &&
                buttons[k].focused ){
            buttons[k].colour.b += 30;
            buttons[k].colour.g += 30;
            buttons[k].colour.r += 30;
            buttons[k].focused = false;

        }
    }
    if((ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT && buttons[k].focused) ||
            (ev->type == SDL_KEYDOWN && ev->key.keysym.sym == SDLK_RETURN && buttons[k].focused && !mode)) {
        buttons[k].pressed = true;
        buttons[k].colour.g -= 20;
        buttons[k].colour.r -= 20;
    }

}

static bool button(button_t *btn) {

    // if button press detected - reset it so it wouldn't trigger twice
    if(btn->pressed) {
        btn->pressed = false;
        return true;
    }
    return false;
}

void tab_controler(SDL_Event evt, button_t *buttons, int *focus, int n) {

    if(evt.type == SDL_KEYUP){
        if(evt.key.keysym.sym == SDLK_DOWN || evt.key.keysym.sym == SDLK_TAB){
            if(buttons[*focus].focused) {
                buttons[*focus].colour.b += 30;
                buttons[*focus].colour.g += 30;
                buttons[*focus].colour.r += 30;
            }
            buttons[*focus].focused = false;
            *focus = (*focus+1)%n;
            buttons[*focus].focused = true;
            buttons[*focus].colour.b -= 30;
            buttons[*focus].colour.g -= 30;
            buttons[*focus].colour.r -= 30;
        } if(evt.key.keysym.sym == SDLK_UP){
            if(buttons[*focus].focused) {
                buttons[*focus].colour.b += 30;
                buttons[*focus].colour.g += 30;
                buttons[*focus].colour.r += 30;
            }
            buttons[*focus].focused = false;
            *focus = (*focus-1+n)%n;
            buttons[*focus].focused = true;
            buttons[*focus].colour.b -= 30;
            buttons[*focus].colour.g -= 30;
            buttons[*focus].colour.r -= 30;
        }

    }

}

int starting_UI(Map *map) {

    // varriables
    int res = 0;
    int quit = 0;
    enum {
        STATE_IN_MENU,
        STATE_IN_GAME,
    } state = 0;

    // checking renderer
    if (map->window == NULL){
        fprintf(stderr, "create pWindow failed: %s\n", SDL_GetError());
        return 1;   // 'error' return status is !0. 1 is good enough
    }
    if(!map->renderer) {   // renderer creation may fail too
        fprintf(stderr, "create renderer failed: %s\n", SDL_GetError());
        return 1;
    }


    // buttons initing
    int mainx = 662, mainy = 300;
    button_t buttons[5];
    for(int i = 0 ; i<5 ; i++){

        buttons[i].pressed = 0;
        buttons[i].focused = false;

        buttons[i].colour.r = 255;
        buttons[i].colour.g = 245 - i*60;
        buttons[i].colour.b = 30;
        buttons[i].colour.a = 255;

        buttons[i].draw_rect.x = mainx - 200;
        buttons[i].draw_rect.y = mainy + i*120;
        buttons[i].draw_rect.h = 100;
        buttons[i].draw_rect.w = 400;


    }

    buttons[4].colour.r = 100;
    buttons[4].colour.g = 100;
    buttons[4].colour.b = 100;
    buttons[4].colour.a = 255;

    buttons[4].draw_rect.x = 20;
    buttons[4].draw_rect.y = 880;
    buttons[4].draw_rect.h = 100;
    buttons[4].draw_rect.w = 200;

    strcpy(buttons[0].text, "One Player");
    strcpy(buttons[1].text, "Two Players");
    strcpy(buttons[2].text, "Three Players");
    strcpy(buttons[3].text, "Four Players");
    strcpy(buttons[4].text, "Settings");




    mainy += 45;
    mainx -= 37;
    int focus = 4;

    // main while
    while(!quit) {

        SDL_Event evt;


        while(SDL_PollEvent(&evt)) {
            // quit on close, pWindow close, or 'escape' key hit
            if(evt.type == SDL_QUIT ||
               (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE) ||
               (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE)) {
                return 0;
            }

            // pass evtent to button
            for(int i = 0 ; i<5 ; i++)
                button_process_event(buttons, i, &evt, &focus, 0);
            tab_controler(evt, buttons, &focus, 5);



        }


        SDL_SetRenderDrawColor(map->renderer, 255, 225, 175, 255);
        SDL_RenderClear(map->renderer);


/*
        TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24); //this opens a font style and sets a size

        SDL_Color White = {0, 0, 0};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

//        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(, "put your text here", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, "amin"); //now you can convert it into a texture

        SDL_Rect Message_rect; //create a rect
        Message_rect.x = 20;  //controls the rect's x coordinate
        Message_rect.y = 20; // controls the rect's y coordinte
        Message_rect.w = 100; // controls the width of the rect
        Message_rect.h = 100; // controls the height of the rect

//Mind you that (0,0) is on the top left of the pWindow/screen, think a rect as the text's box, that way it would be very simple to understance

//Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

        SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

//Don't forget too free your surface and texture
*/
        for(int i = 0 ; i<5 ;i++)
            show_button(map->renderer, buttons+i, map, 0);

        if(state == STATE_IN_MENU) {
            for(int i = 0 ; i<4 ; i++){
                if(button(buttons + i)){
                    res = i+1;
                    state = STATE_IN_GAME;
                }
            }
            if(button(buttons + 4 )){
                SDL_Delay(200);
                settings_UI(map);
                buttons[4].colour.g += 20;
                buttons[4].colour.r += 20;
            }

        } else if(state == STATE_IN_GAME) {
            SDL_Delay(200);
            quit = 1;
        }

        SDL_RenderSetScale(map->renderer, 2, 2);
        stringRGBA(map->renderer, mainx - 405, mainy - 230, "How many players do we have?", black);
        SDL_RenderSetScale(map->renderer, 1, 1);

        SDL_RenderPresent(map->renderer);
    }

    return res;

}

int pause_UI(Map *map){

    int quit = 0;

    enum {
        STATE_IN_MENU,
        STATE_IN_GAME,
    } state = 0;

    // checking map->renderer
    if (map->window == NULL){
        fprintf(stderr, "create pWindow failed: %s\n", SDL_GetError());
        return 0;   // 'error' return status is !0. 1 is good enough
    }
    if(!map->renderer) {   // map->renderer creation may fail too
        fprintf(stderr, "create map->renderer failed: %s\n", SDL_GetError());
        return 0;
    }


    // buttons initing
    int mainx = 662, mainy = 300;
    button_t buttons[5];
    for(int i = 0 ; i<5 ; i++){

        buttons[i].pressed = 0;
        buttons[i].focused = false;

        buttons[i].colour.r = 255;
        buttons[i].colour.g = 245 - i*60;
        buttons[i].colour.b = 30;
        buttons[i].colour.a = 255;

        buttons[i].draw_rect.x = mainx - 200;
        buttons[i].draw_rect.y = mainy + i*120;
        buttons[i].draw_rect.h = 100;
        buttons[i].draw_rect.w = 400;


    }


    buttons[4].colour.r = 100;
    buttons[4].colour.g = 100;
    buttons[4].colour.b = 100;
    buttons[4].colour.a = 255;

    buttons[4].draw_rect.x = 20;
    buttons[4].draw_rect.y = 880;
    buttons[4].draw_rect.h = 100;
    buttons[4].draw_rect.w = 200;


    strcpy(buttons[4].text, "Settings");
    strcpy(buttons[0].text, "Resume");
    strcpy(buttons[1].text, "Restart");
    strcpy(buttons[2].text, "Load last Game");
    strcpy(buttons[3].text, "Quit");

    mainy += 45;
    mainx -= 23;
    int focus = 4;


    // main while
    while(!quit) {

        SDL_Event evt;


        while(SDL_PollEvent(&evt)) {
            // quit on close, pWindow close, or 'escape' key hit
            if(evt.type == SDL_QUIT ||
               (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE)) {
                return 0;
            }
            if(evt.type == SDL_KEYDOWN && (evt.key.keysym.sym == SDLK_ESCAPE || evt.key.keysym.sym == SDLK_BACKSPACE))
                return 1;

            // pass event to button
            for(int i = 0 ; i<5 ; i++)
                button_process_event(buttons, i, &evt, &focus, 0);

            tab_controler(evt, buttons, &focus, 5);


        }


        SDL_SetRenderDrawColor(map->renderer, 255, 225, 175, 255);
        SDL_RenderClear(map->renderer);


        for(int i = 0 ; i<5 ;i++)
            show_button(map->renderer, buttons+i, map, 0);


        if(state == STATE_IN_MENU) {
            if(button(buttons))
                state = STATE_IN_GAME;
            if(button(buttons + 1)){
                map->round = 0;
                map->players = starting_UI(map);
                for(int i = 0 ; i<4 ; i++){
                    map->tanks[i]->score = 0;
                    map->tanks[i]->bullet = 5;
                }
                if(!start_game(map))
                    return 0;
                return 1;
            }
            if(button(buttons + 2)){
                if(load_last_game(map)){
                    start_game(map);
                    state = STATE_IN_GAME;
                }else{
                    buttons[2].focused = false;
                    buttons[2].pressed = false;
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR , "Error","Last game is not Saved.",map->window);
                    buttons[2].colour.g += 50;
                    buttons[2].colour.r += 50;
                    buttons[2].colour.b += 30;
                }
            }
            if(button(buttons + 3))
                return 0;

            if(button(buttons + 4)){
                SDL_Delay(200);
                settings_UI(map);
                buttons[4].colour.g += 20;
                buttons[4].colour.r += 20;
            }



        } else if(state == STATE_IN_GAME) {
            SDL_Delay(200);
            quit = 1;
        }


        SDL_RenderPresent(map->renderer);
    }

    return 1;
}

int settings_UI(Map *map){
    int focus = -1, mode = 0, changing = 0;
    bool is_valid[2] = {true, true};
    int quit = 0;

    enum {
        STATE_IN_MENU,
        STATE_IN_GAME,
    } state = 0;

    // checking map->renderer
    if (map->window == NULL){
        fprintf(stderr, "create pWindow failed: %s\n", SDL_GetError());
        return 0;   // 'error' return status is !0. 1 is good enough
    }
    if(!map->renderer) {   // map->renderer creation may fail too
        fprintf(stderr, "create map->renderer failed: %s\n", SDL_GetError());
        return 0;
    }


    // buttons initing
    button_t keys[22];

    keys[21].pressed = 0;
    keys[21].focused = false;
    strcpy(keys[21].text, "back");
    
    keys[21].colour.r = 120;
    keys[21].colour.g = 120;
    keys[21].colour.b = 120;
    keys[21].colour.a = 255;

    keys[21].draw_rect.x = 20;
    keys[21].draw_rect.y = 880;
    keys[21].draw_rect.h = 100;
    keys[21].draw_rect.w = 200;
    
    

    keys[20].pressed = 0;
    keys[20].focused = false;
    strcpy(keys[20].text, "Change Target");

    keys[20].colour.r = 200;
    keys[20].colour.g = 200;
    keys[20].colour.b = 200;
    keys[20].colour.a = 255;

    keys[20].draw_rect.x = 610;
    keys[20].draw_rect.y = 390;
    keys[20].draw_rect.h = 150;
    keys[20].draw_rect.w = 150;
    


    for(int i = 0 ; i<20 ; i++) {
            keys[i].focused = false;
            keys[i].pressed = 0;
            to_letter(keys[i].text, map->tanks[i/5]->keys[i%5]);

            keys[i].colour.r = 200;
            keys[i].colour.g = 200;
            keys[i].colour.b = 200;
            keys[i].colour.a = 255;

            keys[i].draw_rect.x = 320 + (i/5&1)*650 + deltax[i%5];
            keys[i].draw_rect.y = 220 + (i/5>>1)*400 + deltay[i%5];
            keys[i].draw_rect.h = 91;
            keys[i].draw_rect.w = 91;
        
    }



    // main while
    while(!quit) {
        SDL_Event evt;

        if(SDL_PollEvent(&evt)) {
            // pass event to button
            if (!mode){
                for (int i = 0; i < 22; i++)
                    button_process_event(keys, i, &evt, &focus, mode);
                tab_controler(evt, keys, &focus, 22);
            }
            // quit on close, pWindow close, or 'escape' key hit
            if ((evt.type == SDL_QUIT ||
                 (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE))) {
                return 0;
            }
            if (evt.type == SDL_KEYDOWN && (evt.key.keysym.sym == SDLK_ESCAPE || evt.key.keysym.sym == SDLK_BACKSPACE) && !mode)
                return 1;
            if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_RETURN) {
                if (mode == 21 && is_valid[1]) {
                    keys[20].colour.r = 170;
                    keys[20].colour.g = 170;
                    keys[20].colour.b = 170;
                    mode = 0;
                    is_valid[1] = true;
                }

                if (0 < mode && mode < 21 && is_valid[0]) {
                    map->tanks[(mode - 1) / 5]->keys[(mode - 1) % 5] = changing;
                    keys[mode - 1].colour.r = 170;
                    keys[mode - 1].colour.g = 170;
                    keys[mode - 1].colour.b = 170;
                    mode = 0;
                    is_valid[0] = true;
                }
            }
            char tmp[100];
            to_letter(tmp, changing);
            if (mode && evt.type == SDL_KEYDOWN && evt.key.keysym.scancode != SDL_SCANCODE_RETURN) {
                changing = evt.key.keysym.scancode;
                if (mode == 21) {
                    if (evt.key.keysym.sym == SDLK_UP)
                        map->target++;
                    if (evt.key.keysym.sym == SDLK_DOWN)
                        map->target--;
                    int maxx = 0;
                    for (int i = 0; i < 4; i++)
                        if (map->tanks[i]->score > maxx)
                            maxx = map->tanks[i]->score;
                    if (map->target <= maxx)
                        is_valid[1] = false;
                    else
                        is_valid[1] = true;

                } else if (mode && strcmp(tmp, "\0")) {
                    bool flag = true;
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 5; j++)
                            if (changing == map->tanks[i]->keys[j] && 5 * i + j + 1 != mode)
                                flag = false;
                    to_letter(keys[mode - 1].text, changing);
                    if (flag)
                        is_valid[0] = true;
                    else
                        is_valid[0] = false;
                }
            }



        }

        SDL_SetRenderDrawColor(map->renderer, 255, 225, 175, 255);
        SDL_RenderClear(map->renderer);

        for(int i = 0 ; i<22 ; i++)
            show_button(map->renderer, &keys[i], map, (i+1 == mode)*!is_valid[i/20]);

        if(state == STATE_IN_MENU && !mode) {
            if(button(&keys[21]))
                state = STATE_IN_GAME;
            for(int i = 0 ; i<21 ; i++)
                if(button(&keys[i])){
                    keys[i].colour.b = 200;
                    keys[i].colour.g = 50;
                    keys[i].colour.r = 50;
                    mode = i+1;
                }
        } else if(state == STATE_IN_GAME) {
            SDL_Delay(200);
            quit = 1;
        }
        SDL_RenderSetScale(map->renderer, 1.4,1.4);
        stringRGBA(map->renderer, 326/1.4, 100/1.4, "Player 1", black);
        stringRGBA(map->renderer, 976/1.4, 100/1.4, "Player 2", black);
        stringRGBA(map->renderer, 326/1.4, 500/1.4, "Player 3", black);
        stringRGBA(map->renderer, 976/1.4, 500/1.4, "Player 4", black);

        char tmp[20];
        sprintf(tmp , "%d", map->target);
        SDL_RenderSetScale(map->renderer, 1.3,1.3);
        if(mode && !is_valid[1])
            stringRGBA(map->renderer, 675/1.3, 480/1.3, tmp, 100, 0, 0 , 255);
        else
            stringRGBA(map->renderer, 675/1.3, 480/1.3, tmp, black);
        SDL_RenderSetScale(map->renderer, 1,1);

        SDL_RenderPresent(map->renderer);

    }

    return 1;
}

