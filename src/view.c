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
#include <zconf.h>
#include "view.h"
#include "structs.h"
#include "physics.h"
#include "logic.h"
#include "init.h"

#define black 0,0 ,0, 255


//Uint32 getpixel(SDL_Surface *surface, int x, int y)
//{
//    int bpp = surface->format->BytesPerPixel;
//    /* Here p is the address to the pixel we want to retrieve */
//    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
//
//    switch(bpp) {
//        case 1:
//            return *p;
//            break;
//
//        case 2:
//            return *(Uint16 *)p;
//            break;
//
//        case 3:
//            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
//                return p[0] << 16 | p[1] << 8 | p[2];
//            else
//                return p[0] | p[1] << 8 | p[2] << 16;
//            break;
//
//        case 4:
//            return *(Uint32 *)p;
//            break;
//
//        default:
//            return 0;       /* shouldn't happen, but avoids warnings */
//    }
//}


#define red 255, 0, 0 , 255

void show_tank(Tank *t, SDL_Renderer *renderer) {
    double a = t->angle, x = t->x, y = t->y;
    thickLineColor(renderer, x + t->lenght/2 * cos(a), y - t->lenght/2 * sin(a), x - t->lenght/2 * cos(a),
            y + t->lenght/2 * sin(a), t->width, t->light_color);                                                    //shasi
    thickLineColor(renderer, x, y, x + t->barrel_lenght * cos(a), y - t->barrel_lenght * sin(a),
                   t->barrel_thickness, t->dark_color);                                                                    //barrel
    thickLineColor(renderer, x+t->lenght/2*cos(a)+0.4*t->width*sin(a) ,
                   y - t->lenght/2 * sin(a) + 0.4*t->width * cos(a), x - t->lenght/2 * cos(a) + 0.4*t->width * sin(a),
                   y + t->lenght/2 * sin(a) + 0.4*t->width * cos(a), 0.25*t->width, t->dark_color);                        //sheni 1
    thickLineColor(renderer, x+t->lenght/2*cos(a)-0.4*t->width*sin(a) ,
                   y - t->lenght/2 * sin(a) - 0.4*t->width * cos(a), x - t->lenght/2 * cos(a) - 0.4*t->width * sin(a),
                   y + t->lenght/2 * sin(a) - 0.4*t->width * cos(a), 0.25*t->width, t->dark_color);                        //sheni 2
    filledCircleColor(renderer, x, y, 0.4*t->width, t->dark_color);                                                 //circle
    circleRGBA(renderer, x, y, 0.4*t->width, black);                                                                //circle border

}

int to_int(char buff[5]) {
    int res = 0;
    char* it = buff;
    while(*it != '\0'){
        res = 10*res + *it - '0';
        it++;
    }
    return res;
}

void show_walls(Map *map, struct SDL_Renderer * renderer){
    SDL_Rect rect ;
    rect.x = 1.*(960-map->ratio*map->maxx)/2 + 20;
    rect.y = 1.*(960-map->ratio*map->maxy)/2 + 20;
    rect.h = map->ratio*map->maxy;
    rect.w = map->ratio*map->maxx;
    SDL_SetRenderDrawColor(renderer, 160, 207, 255, 255);
    SDL_RenderFillRect(renderer,&rect);
    for(int i = 0 ; i<map->count_of_walls ; i++)
        thickLineRGBA(renderer, map->walls[i]->pos[0], map->walls[i]->pos[1], map->walls[i]->pos[2], map->walls[i]->pos[3], 0.03*map->ratio, 0, 41, 102, 255);
}

void walls_to_file(Cell **cells, int n, int m) {
    FILE *map_prim ;
    if(!(map_prim = fopen("map_prim.txt", "w")))
        perror("map_prim not created.\n");
    int counter = 2;
    for (int i = 0; i < m; ++i) {
        int j = 0 ;
        while(j<n){
            while(j<n && cells[j][i].access[3])
                j++;
            int tmp = j;
            while(j<n && !cells[j][i].access[3])
                j++;
            counter++;
            if(tmp!=j)
                fprintf(map_prim, "%d %d %d %d\n", i, tmp, i, j);
        }
    }
    for (int i = 0; i < n; ++i) {
        int j = 0;
        while (j < m) {
            while (j < m && cells[i][j].access[0])
                j++;
            int tmp = j;
            while (j < m && !cells[i][j].access[0])
                j++;
            counter++;
            if(tmp!=j)
                fprintf(map_prim, "%d %d %d %d\n", tmp, i, j, i);
        }
    }
    fprintf(map_prim, "%d 0 %d %d\n",  m,m,n );
    fprintf(map_prim, "0 %d %d %d\n",  n,m,n );

    char ch;
    FILE *map = fopen("map.txt", "w");
    fprintf(map, "%d\n", counter);
    fclose(map_prim);
    map_prim = fopen("map_prim.txt", "r");
    while( ( ch = fgetc(map_prim) ) != EOF )
        fputc(ch, map);
    remove("map_prim.txt");
    fclose(map);
    fclose(map_prim);
}

void show_bullet(Map *map, SDL_Renderer *renderer) {
    Bullet_Node *b_node = map->bullets;
    srand(time(NULL));
    while(b_node != NULL){
        if(b_node->b.life_time == 0){
            b_node = b_node->next;
            /// TODO cut the linked list
            continue;
        }
        switch (meet_wall(map, b_node->b)){
            case 1:
                b_node->b.angle *= -1;
                break;
            case 2:
                b_node->b.angle = M_PI - b_node->b.angle;
                break;
            default:
                break;
        }
        filledCircleRGBA(renderer, (Sint16) b_node->b.x, (Sint16) b_node->b.y, (Sint16) b_node->b.radius, black);
        b_node->b.life_time--;
        b_node->b.x += 1.2 *cos(b_node->b.angle);
        b_node->b.y += 1.2 *sin(b_node->b.angle);
        if(meet_tank(map, b_node->b)){
            b_node->b.life_time = 0;
            ///todo terminate this bullet
        }
        b_node = b_node->next;
    }
}

void show_scores(Map *map, SDL_Renderer *renderer) {
    SDL_Rect rect;
    rect.x = 1000;
    rect.y = 0;
    rect.h = 1040;
    rect.w = 2000;
    SDL_SetRenderDrawColor(renderer, 89, 190, 255, 255);
    SDL_RenderFillRect(renderer,&rect);
    char s[20] ;
    sprintf(s, "Round noumber: %d", map->round);
    SDL_RenderSetScale(map->renderer, 1.2, 1.2);
    stringRGBA(renderer, 1105/1.2, 20/1.2, s, 0, 33, 168, 255);
    SDL_RenderSetScale(map->renderer, 1, 1);
    double x = 1120, y = 160;
    for(int i = 0 ; i<map->players ; i++){
        Tank* tmp_tank = malloc(sizeof(Tank));
        tmp_tank->bullet = map->tanks[i]->bullet;
        tmp_tank->lenght = 150;
        tmp_tank->width = 125;
        tmp_tank->barrel_lenght = 100;
        tmp_tank->barrel_thickness = 25;
        switch(i){
            case 0:
                tmp_tank->light_color =  255 | (95 << 8) | (66 << 16) | (255 << 24);
                tmp_tank->dark_color = 255 | (255 << 24);
                break;
            case 1:
                if(map->ai_mode){
                    tmp_tank->light_color =  100 | (100 << 8) | (100<< 16) | (255 << 24);
                    tmp_tank->dark_color = 50 | (50 << 8) | (50<< 16) | (255 << 24);
                } else{
                    tmp_tank->light_color =  66 | (134 << 8) | (244<< 16) | (255 << 24);
                    tmp_tank->dark_color = 0 | (80 << 8) | (255<< 16) | (255 << 24);
                }
                break;
            case 2:
                tmp_tank->light_color =  243 | (112 << 8) | (255 << 16) | (255 << 24);
                tmp_tank->dark_color = 187 | (0 << 8) | (255 << 16) | (255 << 24);
                break;
            case 3:
                tmp_tank->light_color =  155 | (255 << 8) | (106 << 16) | (255 << 24);
                tmp_tank->dark_color = 0 | (255 << 8) | (38<< 16) | (255 << 24);
                break;
        }
        tmp_tank->x = x;
        tmp_tank->y = y;
        tmp_tank->angle = M_PI/2;
        show_tank(tmp_tank, renderer);
        if(map->tanks[i]->power_up->model)
            filledCircleRGBA(map->renderer, tmp_tank->x, tmp_tank->y - tmp_tank->barrel_lenght ,10, black);
        *s = '\0';
        sprintf(s,"%s%d","Score: ",map->tanks[i]->score);
        if(map->tanks[i]->is_alive)
            stringRGBA(renderer, x+80, y-15, s ,black);
        else
            stringRGBA(renderer, x+80, y-15, s ,red);
        *s = '\0';
        sprintf(s,"%s%d","Bullets: ",map->tanks[i]->bullet);
        if(map->tanks[i]->is_alive)
            stringRGBA(renderer, x+80, y+10, s ,black);
        else
            stringRGBA(renderer, x+80, y+10, s ,red);
        y += 50+tmp_tank->lenght/2 + tmp_tank->barrel_lenght;
    }

    x = 1118, y = 158;
    for(int i = 0 ; i<map->players-map->ai_mode ; i++){
        char tmp[100], tmp2[100], tmp3[100];
        to_letter(tmp, map->tanks[i]->keys[2]);
        stringRGBA(renderer, x-strlen(tmp), y-20, tmp, black);
        to_letter(tmp, map->tanks[i]->keys[1]);
        to_letter(tmp2, map->tanks[i]->keys[4]);
        to_letter(tmp3, map->tanks[i]->keys[0]);
        sprintf(tmp, "%s %s %s", tmp, tmp2, tmp3);
        stringRGBA(renderer, x-3*strlen(tmp),y, tmp, black);
        to_letter(tmp, map->tanks[i]->keys[3]);
        stringRGBA(renderer, x-strlen(tmp), y+20, tmp, black);
        y += 225;
    }
}

void *to_letter(char *result,int i) {

    switch (i){
        case 4:
            strcpy(result, "A");
            break;
        case 5:
            strcpy(result, "B");
            break;
        case 6:
            strcpy(result, "C");
            break;
        case 7:
            strcpy(result, "D");
            break;
        case 8:
            strcpy(result, "E");
            break;
        case 9:
            strcpy(result, "F");
            break;
        case 10:
            strcpy(result, "G");
            break;
        case 11:
            strcpy(result, "H");
            break;
        case 12:
            strcpy(result, "I");
            break;
        case 13:
            strcpy(result, "J");
            break;
        case 14:
            strcpy(result, "K");
            break;
        case 15:
            strcpy(result, "L");
            break;
        case 16:
            strcpy(result, "M");
            break;
        case 17:
            strcpy(result, "N");
            break;
        case 18:
            strcpy(result, "O");
            break;
        case 19:
            strcpy(result, "P");
            break;
        case 20:
            strcpy(result, "Q");
            break;
        case 21:
            strcpy(result, "R");
            break;
        case 22:
            strcpy(result, "S");
            break;
        case 23:
            strcpy(result, "T");
            break;
        case 24:
            strcpy(result, "U");
            break;
        case 25:
            strcpy(result, "V");
            break;
        case 26:
            strcpy(result, "W");
            break;
        case 27:
            strcpy(result, "X");
            break;
        case 28:
            strcpy(result, "Y");
            break;
        case 29:
            strcpy(result, "Z");
            break;
        case 30:
            strcpy(result, "1");
            break;
        case 31:
            strcpy(result, "2");
            break;
        case 32:
            strcpy(result, "3");
            break;
        case 33:
            strcpy(result, "4");
            break;
        case 34:
            strcpy(result, "5");
            break;
        case 35:
            strcpy(result, "6");
            break;
        case 36:
            strcpy(result, "7");
            break;
        case 37:
            strcpy(result, "8");
            break;
        case 38:
            strcpy(result, "9");
            break;
        case 39:
            strcpy(result, "0");
            break;
        case 43:
            strcpy(result, "Tab");
            break;
        case 44:
            strcpy(result, "Space");
            break;
        case 45:
            strcpy(result, "-");
            break;
        case 46:
            strcpy(result, "=");
            break;
        case 47:
            strcpy(result, "[");
            break;
        case 48:
            strcpy(result, "]");
            break;
        case 49:
            strcpy(result, "\\");
            break;
        case 51:
            strcpy(result, ";");
            break;
        case 52:
            strcpy(result, "'");
            break;
        case 53:
            strcpy(result, "`");
            break;
        case 54:
            strcpy(result, ",");
            break;
        case 55:
            strcpy(result, ".");
            break;
        case 56:
            strcpy(result, "/");
            break;
        case 79:
            strcpy(result, "->");
            break;
        case 80:
            strcpy(result, "<-");
            break;
        case 81:
            strcpy(result, "\\/");
            break;
        case 82:
            strcpy(result, "/\\");
            break;
        default:
            strcpy(result, "\0");
            break;
    }

}

void show_button(SDL_Renderer *r, button_t *btn, Map *map, bool error) {
    if(error)
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    else
        SDL_SetRenderDrawColor(r, btn->colour.r, btn->colour.g, btn->colour.b, btn->colour.a);
    SDL_RenderFillRect(r, &btn->draw_rect);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderDrawRect(r, &btn->draw_rect);
    SDL_RenderSetScale(map->renderer, 1.3, 1.3);
    if(error)
        stringRGBA(map->renderer, (btn->draw_rect.x + btn->draw_rect.w/2 - 5* strlen(btn->text))/1.3,
                   (btn->draw_rect.y + btn->draw_rect.h/2 -3)/1.3 , btn->text, 100, 0,0, 150);
    else
        stringRGBA(map->renderer, (btn->draw_rect.x + btn->draw_rect.w/2 - 5* strlen(btn->text))/1.3,
                   (btn->draw_rect.y + btn->draw_rect.h/2 -3)/1.3 , btn->text, black);
    SDL_RenderSetScale(map->renderer, 1, 1);

}

void show_power_up(Map *map, power_up *param) {
    filledCircleRGBA(map->renderer, param->x, param->y , 0.05*map->ratio, black);
//    SDL_Surface * image;
//    switch (param.model){
//        case 1:
//            image = SDL_LoadBMP("Mine.jpg");
//
//            break;
//        default:
//            break;
//    }
//    SDL_Texture * texture = SDL_CreateTextureFromSurface(map->renderer, image);
//    SDL_RenderCopy(map->renderer, texture, NULL, NULL);
//    SDL_DestroyTexture(texture);
//    SDL_FreeSurface(image);
}


