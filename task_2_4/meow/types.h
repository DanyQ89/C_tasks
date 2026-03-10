#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} App;

typedef struct {
    bool up, down, right, left, enter, reset, quit, space;
} Command_pressed;

typedef struct {
    int x;
    int y;
} Coords;

typedef struct {
    int width;
    int height;
} Size;

typedef struct {
    Coords position;
    SDL_Color color;
} Window;

typedef struct {
    Coords position;
    SDL_Color color;
    SDL_Color highlight_color;
    char name[256];
} Button;

typedef struct {
    Button buttons[16];
    int count;
} ButtonList;

typedef struct {
    Window window;
    ButtonList buttons_list;
} Menu;

typedef struct {
    Coords position;
    Coords velocity;
    SDL_Color color;
    int radius;
} Ball;

typedef struct {
    Ball balls_list[9];
    int count;
} BallList;

typedef struct {
    Size size;
    Coords position;
    SDL_Color color;
    int health;
} Block;

typedef struct {
    Block block_list[60];
    int count;
    Block vaus;
} BlockList;

typedef struct {
    char* levels[10];
    int count;
} LevelList;

#endif