#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <SDL.h>


typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} App;

typedef struct {
    bool up, down, right, left, enter, reset, quit;
} Command_pressed;

typedef struct {
    int x;
    int y;
} Coords;


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
} ButtonsList;

typedef struct {
    Window window;
    ButtonsList buttons_list;
} Menu;

enum {
    LEFT = SDL_SCANCODE_LEFT,
    RIGHT = SDL_SCANCODE_RIGHT,
    UP = SDL_SCANCODE_UP,
    DOWN = SDL_SCANCODE_DOWN,
    ENTER = SDL_SCANCODE_RETURN,
    RESET = SDL_SCANCODE_R,
    QUIT = SDL_SCANCODE_ESCAPE
};

//main
App SystemOpen(void);
void SystemClose(SDL_Renderer* render, SDL_Window* window);


// draw
void draw_first(SDL_Renderer* ren);
void draw_button(SDL_Renderer* ren, Button button, bool focused);
void make_window(SDL_Window* AppWindow, SDL_Renderer* ren, Window window);
void show_window(SDL_Renderer* ren);
void draw_buttons(SDL_Renderer* ren, ButtonsList buttons, int focused, SDL_Color c);
// interaction
Command_pressed get_command(const Uint8* keystate);
char* SystemUpdate(SDL_Window* window, SDL_Renderer* ren, Menu menu);

//parsing
void SystemRelease(Menu* menu);
Menu SystemInitialise(char* filename);


#endif