#ifndef HEADER_H


#define HEADER_H

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
#define STEPS_CHECK 1

extern TTF_Font* font_button;
extern TTF_Font* font_block;

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
    Coords velocity; // вектор направления движения
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


typedef struct {
    BlockList blocks;
    BallList balls;
    bool ball_released;
    int life_cnt;
} Game;

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

// ==================== MAIN ====================
App SystemOpen(void);
void SystemClose(SDL_Renderer* render, SDL_Window* window);
void SystemUpdate(SDL_Window* window, SDL_Renderer* ren, Menu menu);

// ==================== DRAW ====================
void draw_first(SDL_Renderer* ren);
void draw_button(SDL_Renderer* ren, Button button, bool focused);
void draw_buttons(SDL_Renderer* ren, ButtonList buttons, int focused, SDL_Color c);
void draw_block(SDL_Renderer* ren, Block block);
void draw_blocks(SDL_Renderer* ren, BlockList blocks);
void draw_ball(SDL_Renderer* ren, Ball ball);
void draw_balls(SDL_Renderer* ren, BallList balls);
void draw_blocks_and_balls(SDL_Renderer* ren, BlockList blocks, BallList balls, SDL_Color c, int lives_left);
void make_window(SDL_Window* AppWindow, SDL_Renderer* ren, Window window);
void show_window(SDL_Renderer* ren);
void draw_text(SDL_Renderer* ren, const char* text, int x, int y, SDL_Color color, TTF_Font* font);
void init_fonts(void);
void close_fonts(void);

// ==================== PARSING ====================
Menu SystemInitialize(char* filename);
void SystemRelease(Menu* menu);
BlockList LevelInitialize(char* filename);

// ==================== LOGIC ====================
void set_new_game(SDL_Renderer* ren, BlockList* blocks, BallList* balls, SDL_Color c);
void launch_balls(BallList* balls);
void move_balls(BallList* balls);
void check_balls_collision(BallList* balls, BlockList* blocks);
void remove_balls(BallList* balls, BlockList* blocks, bool* ball_released, int* lives_left);
char* update_game(BallList* balls, BlockList* blocks, bool* ball_released, int* lives_left);
bool have_blocks(BlockList* blocks);
BallList reset_balls(BlockList* blocks);

// ==================== COMMAND ====================
Command_pressed get_command(const Uint8* keystate);

#endif