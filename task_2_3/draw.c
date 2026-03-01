#include "header.h"
#include <SDL.h>
#include <SDL_ttf.h>

#define COLOR_RENDER_R 181
#define COLOR_RENDER_G 101
#define COLOR_RENDER_B 167
#define COLOR_RENDER_A 255

#define COLOR_LINE_R 0
#define COLOR_LINE_G 0
#define COLOR_LINE_B 0
#define COLOR_LINE_A 255

#define default_width 180
#define default_height 100

SDL_Color color_render = { .r = COLOR_RENDER_R, .g = COLOR_RENDER_G, .b = COLOR_RENDER_B, .a = COLOR_RENDER_A };
SDL_Color color_line = { .r = COLOR_LINE_R, .g = COLOR_LINE_G, .b = COLOR_LINE_B, .a = COLOR_LINE_A };


void draw_first(SDL_Renderer* ren) {
    SDL_Color c = color_render;
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
    SDL_RenderClear(ren);
}

void draw_text(SDL_Renderer* ren, const char* text, int x, int y, SDL_Color color) {
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
    
    if (!font) return;
    
    SDL_Surface* surf = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);

    SDL_Rect dst = { x, y, surf->w, surf->h };
    SDL_RenderCopy(ren, tex, NULL, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
    TTF_CloseFont(font);
}

void draw_button(SDL_Renderer* ren, Button button, bool focused) {
    SDL_Color c, tc;
    if (focused) {
        c = button.highlight_color;
        tc = button.color;
    }
    else {
        c = button.color;
        tc = button.highlight_color;
    }

    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);

    SDL_Rect buttonRect =
    {
        button.position.x,
        button.position.y,
        default_width,
        default_height
    };

    SDL_RenderFillRect(ren, &buttonRect);

    draw_text(ren, button.name, button.position.x, button.position.y, tc);


}

void draw_buttons(SDL_Renderer* ren, ButtonsList buttons, int focused, SDL_Color c) {
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, 255);
    SDL_RenderClear(ren);

    for (int i = 0; i < buttons.count; i++) {

        if (focused == i) {

            draw_button(ren, buttons.buttons[i], true);
        }
        else {
            draw_button(ren, buttons.buttons[i], false);
        }
    }

    SDL_RenderPresent(ren);

}


void make_window(SDL_Window* AppWindow, SDL_Renderer* ren, Window window) {
    SDL_Color c = window.color;

    SDL_SetWindowPosition(AppWindow, window.position.x, window.position.y);
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, 0);
    SDL_RenderClear(ren);
    //SDL_RenderPresent(ren);
}

void show_window(SDL_Renderer* ren) {
    SDL_RenderPresent(ren);
}



