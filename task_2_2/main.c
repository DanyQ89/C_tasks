#include <stdio.h> 
#include <SDL.h>
#include <stdbool.h>
#include "header.h"

#define width 500
#define height 500

App init() {
    App app = { NULL, NULL };

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        return app;
    }

    // окно

    app.window = SDL_CreateWindow(
        "Serpinsky",
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

    return app;
}

int main() {
    App app = init();
    if (app.window == NULL || app.renderer == NULL) {
        return 1;
    }

    SDL_Renderer* ren = app.renderer;

    bool running = true;
    SDL_Event event = { 0 };
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    InputStateMode mode = WAITING_FIRST_CLICK;
    Point start = { 0, 0 };
    Point end = { 0, 0 };
    int now_x = 0, now_y = 0;

    array.now_iter = 0;

    draw_first(ren);
    SDL_RenderPresent(ren);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_KEYDOWN && mode == WORKING) {
                if (event.key.keysym.scancode == LEFT || event.key.keysym.scancode == A) {
                    go_to_iter(-1);
                }
                if (event.key.keysym.scancode == RIGHT || event.key.keysym.scancode == D) {
                    go_to_iter(1);
                }
            }

            MouseResult res = handle_mouse_event(event, mode, start, end, now_x, now_y);
            mode = res.mode;
            start = res.start;
            end = res.end;
            now_x = res.now_x;
            now_y = res.now_y;
        }

        Command_pressed cmd = get_command(keystate);

        if (cmd.quit) {
            running = false;
        }
        if (cmd.reset) {
            mode = WAITING_FIRST_CLICK;
            array.now_iter = 0;
            draw_first(ren);
            start.x = 0;
            start.y = 0;


            SDL_RenderPresent(ren);
        }

        if (mode == WAITING_SECOND_CLICK && (now_x != 0 || now_y != 0)) {
            draw_first(ren);
            draw_line(ren, start, end);
            SDL_RenderPresent(ren);
        }

        if (mode == WORKING) {
            draw_first(ren);

            // по пифагору длину вычисляем как гип
            double base_length = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));

            sierpinski_arrowhead_curve(ren, start, end, base_length, array.now_iter);

            SDL_RenderPresent(ren);
        }

        

    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
    return 0;
}