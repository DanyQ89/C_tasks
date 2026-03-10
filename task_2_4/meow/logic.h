#pragma once
#ifndef LOGIC_H
#define LOGIC_H

#include <SDL.h>
#include "types.h"
#include "constants.h"

void set_new_game(SDL_Renderer* ren, BlockList* blocks, BallList* balls, SDL_Color c);
void launch_balls(BallList* balls);
void move_balls(BallList* balls);
void check_balls_collision(BallList* balls, BlockList* blocks);
void remove_balls(BallList* balls, BlockList* blocks, bool* ball_released, int* lives_left);
char* update_game(BallList* balls, BlockList* blocks, bool* ball_released, int* lives_left);
bool have_blocks(BlockList* blocks);
BallList reset_balls(BlockList* blocks);

#endif