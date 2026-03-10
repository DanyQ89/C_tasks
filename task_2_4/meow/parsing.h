#pragma once
#ifndef PARSING_H
#define PARSING_H

#include "types.h"

Menu SystemInitialize(char* filename);
void SystemRelease(Menu* menu);
BlockList LevelInitialize(char* filename);

#endif