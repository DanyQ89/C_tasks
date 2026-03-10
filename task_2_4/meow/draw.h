#ifndef DRAW_H
#define DRAW_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "types.h"
#include "constants.h"

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
void draw_text(SDL_Renderer* ren, const char* text, int x, int y, int width, int height, SDL_Color color, TTF_Font* font);
void init_fonts(void);
void close_fonts(void);

#endif