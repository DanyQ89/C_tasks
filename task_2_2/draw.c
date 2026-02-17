#include <SDL.h>
#include "header.h"
#define COLOR_RENDER_R 181
#define COLOR_RENDER_G 101
#define COLOR_RENDER_B 167
#define COLOR_RENDER_A 255

#define COLOR_LINE_R 0
#define COLOR_LINE_G 0
#define COLOR_LINE_B 0
#define COLOR_LINE_A 255

SDL_Color color_render = { .r = COLOR_RENDER_R, .g = COLOR_RENDER_G, .b = COLOR_RENDER_B, .a = COLOR_RENDER_A };
SDL_Color color_line = { .r = COLOR_LINE_R, .g = COLOR_LINE_G, .b = COLOR_LINE_B, .a = COLOR_LINE_A };


void draw_first(SDL_Renderer* ren) {
    SDL_Color c = color_render;
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
    SDL_RenderClear(ren);
}

void draw_line(SDL_Renderer* ren, Point start, Point end) {
    SDL_Color c = color_line;
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
    printf("%d %d %d %d asd\n", start.x, start.y, end.x, end.y);

    int res = SDL_RenderDrawLine(ren, start.x, start.y, end.x, end.y);
    printf("%d", res);
}


void draw_line_double(SDL_Renderer* ren, Point start, Point end) {
    SDL_Color c = color_line;
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
    SDL_RenderDrawLine(ren, (int)start.x, (int)start.y, (int)end.x, (int)end.y);
}
