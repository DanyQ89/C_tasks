#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <SDL.h>
#include <stdbool.h>
 
#define MAX_ITERATIONS 20
#define MAX_LINES_PER_ITER 100000

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} App;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point start;
    Point end;
} Line;

typedef struct {
    Line lines[MAX_LINES_PER_ITER];
    int count;
} LineList;

typedef struct {
    LineList iterations[MAX_ITERATIONS];
    int now_iter;
} IterationsList;

typedef struct {
    bool up, down, right, left, quit, reset;
} Command_pressed;

typedef enum {
    WAITING_FIRST_CLICK,
    WAITING_SECOND_CLICK,
    WORKING
} InputStateMode;

typedef struct {
    InputStateMode mode;
    Point start;
    Point end;
    int now_x;
    int now_y;
} MouseResult;

enum {
    A = SDL_SCANCODE_A,
    D = SDL_SCANCODE_D,
    LEFT = SDL_SCANCODE_LEFT,
    RIGHT = SDL_SCANCODE_RIGHT,
    ENTER = SDL_SCANCODE_RETURN,
    R = SDL_SCANCODE_R
};

void draw_first(SDL_Renderer* ren);
void draw_line(SDL_Renderer* ren, Point start, Point end);
void draw_line_double(SDL_Renderer* ren, Point start, Point end);
void sierpinski_arrowhead_curve(SDL_Renderer* ren, Point start, Point end, double base_length, unsigned order);
Command_pressed get_command(const Uint8* keystate);
MouseResult handle_mouse_event(SDL_Event event, InputStateMode mode, Point start, Point end, int now_x, int now_y);
void set_new_triangle(Point start, Point end);
void go_to_iter(int iter);
void make_new_iter(int iter);
IterationsList array;

#endif