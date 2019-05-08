//
// Created by amin on 12/18/18.
//


#ifndef PROJECT_STRUCTS_H
#define PROJECT_STRUCTS_H

#include <SDL2/SDL.h>
#include <stdbool.h>


typedef struct {
    int life;
    double x, y;
    bool is_activated;
} Mine;

typedef struct {
    double x, y;
}Point;

typedef enum {
    EMine = 1,
    ELaser,
    EFrag_bomb
} Power_up;

typedef struct {
    double x, y;
    Power_up model;
}power_up;

typedef struct {
    int bullet;
    double x, y, angle;
    double radius, barrel_lenght, barrel_thickness, width, lenght;
    int light_color, dark_color;
    int score;
    Point corners[5];
    bool is_alive;
    int keys[5];
    int refresh_time;
    power_up *power_up;
}Tank;

typedef struct {
    double x, y, angle;
    int radius, life_time;
}Bullet;

typedef struct Bullet_Node{
    Bullet b;
    struct Bullet_Node* next;
}Bullet_Node;

typedef struct {
    SDL_Rect draw_rect;    // dimensions of button
    struct {
        Uint8 r, g, b, a;
    } colour;
    bool focused;
    int pressed;
    char text[100];
} button_t;

typedef enum{
    VERTICAL, HORIZONTAL
}direction;

typedef struct {
    double pos[4];
    direction dir;
}Wall;

typedef struct {
    Tank **tanks;
    Wall **walls;
    Bullet_Node *bullets;
    int round;
    int players;
    int count_of_walls;
    int count_of_bullets;
    int target;
    int max_point;
    double maxx, maxy, ratio;
    int end_time;
    bool ai_mode;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Mine **mines;
    int mines_number;
    int  powerup_time;
    power_up **power_ups;
    int power_ups_number;

}Map;

typedef struct {
    Point p[2];
}Line;

typedef struct {
    Uint8* position;
    Uint32 length;
}AudioData;

typedef struct {
    bool checked;
    int access[4];
} Cell;


#endif //PROJECT_STRUCTS_H
