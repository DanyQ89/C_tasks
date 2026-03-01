#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <windows.h>
#include <locale.h>
#include "header.h"
#include <SDL.h>
#include <SDL_ttf.h>


#define width 1000
#define height 1000

App SystemOpen() {
    App app = { NULL, NULL };

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        return app;
    }



    // окно

    app.window = SDL_CreateWindow(
        "Meow meow",
        //SDL_WINDOWPOS_CENTERED,
        //SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        0
    );

    // создает для ускорения работы окна и показывает его
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    //Uint32 render_flags = SDL_RENDERER_PRESENTVSYNC;

    app.renderer = SDL_CreateRenderer(app.window, -1,
        render_flags);

    if (app.window == NULL || app.renderer == NULL) {
        printf("Ошибка при создании файла");
        return app;
    }
    return app;
}

void SystemClose(SDL_Renderer* render, SDL_Window* window) {
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


int main() {
    
    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }
    App app = SystemOpen();


    //SystemInitialize("menu_main.txt");

    Menu main_menu = SystemInitialise("./menu_main.txt");

    char* choice = SystemUpdate(app.window, app.renderer, main_menu);
    printf("Was chosen: %s\n", choice);

    SystemRelease(&main_menu);
   

    SystemClose(app.renderer, app.window);
    return 0;
}



