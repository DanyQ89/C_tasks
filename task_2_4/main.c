#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <windows.h>
#include <locale.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "../meow/meow/main.h"
#include "../meow/meow/types.h"
#include "../meow/meow/constants.h"

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
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    // создает для ускорения работы окна и показывает его
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
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
    close_fonts();
    TTF_Quit();
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


int main() {

    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }
    init_fonts();
    App app = SystemOpen();


    //SystemInitialize("menu_main.txt");

    //Menu main_menu = SystemInitialize("./menu_main.txt");
    Menu main_menu = SystemInitialize("C:/Users/danil/PycharmProjects/pythonProject/venv/codes/help/meow/menu_main.txt");
    SystemUpdate(app.window, app.renderer, main_menu);
    //char* choice = SystemUpdate(app.window, app.renderer, main_menu);

    //printf("Was chosen: %s\n", choice);

    SystemRelease(&main_menu);


    SystemClose(app.renderer, app.window);
    return 0;
}
