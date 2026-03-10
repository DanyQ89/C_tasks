#pragma once
#ifndef MAIN_H
#define MAIN_H

#include "constants.h"
#include "types.h"
#include "draw.h"
#include "parsing.h"
#include "logic.h"

// ==================== MAIN ====================
App SystemOpen(void);
void SystemClose(SDL_Renderer* render, SDL_Window* window);
void SystemUpdate(SDL_Window* window, SDL_Renderer* ren, Menu menu);

// ==================== COMMAND ====================
Command_pressed get_command(const Uint8* keystate);

#endif