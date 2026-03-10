#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700
#define BALL_RADIUS 10
#define VAUS_WIDTH 200
#define VAUS_HEIGHT 20
#define VAUS_SPEED 10
#define BALL_ANGLE 60
#define BALL_SPEED 10
#define LIVES_HAVE 5
#define STEPS_CHECK 5

enum {
    LEFT = SDL_SCANCODE_LEFT,
    RIGHT = SDL_SCANCODE_RIGHT,
    UP = SDL_SCANCODE_UP,
    DOWN = SDL_SCANCODE_DOWN,
    ENTER = SDL_SCANCODE_RETURN,
    RESET = SDL_SCANCODE_R,
    QUIT = SDL_SCANCODE_ESCAPE,
    SPACE = SDL_SCANCODE_SPACE
};

#endif