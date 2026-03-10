#include "header.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "../meow/meow/draw.h"
#include "../meow/meow/types.h"
#include "../meow/meow/constants.h"

#define COLOR_RENDER_R 181
#define COLOR_RENDER_G 101
#define COLOR_RENDER_B 167
#define COLOR_RENDER_A 255

#define COLOR_LINE_R 0
#define COLOR_LINE_G 0
#define COLOR_LINE_B 0
#define COLOR_LINE_A 255

#define default_width 300
#define default_height 150

TTF_Font* font_button = NULL;
TTF_Font* font_block = NULL;

SDL_Color color_render = { .r = COLOR_RENDER_R, .g = COLOR_RENDER_G, .b = COLOR_RENDER_B, .a = COLOR_RENDER_A };
SDL_Color color_line = { .r = COLOR_LINE_R, .g = COLOR_LINE_G, .b = COLOR_LINE_B, .a = COLOR_LINE_A };


void draw_first(SDL_Renderer* ren) {
    SDL_Color c = color_render;
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
    SDL_RenderClear(ren);
}

void init_fonts(void) {
    if (!font_button) {
        font_button = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 40);  
    }
    if (!font_block) {
        font_block = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 18);   
    }
}

void close_fonts(void) {
    if (font_button) TTF_CloseFont(font_button);
    if (font_block) TTF_CloseFont(font_block);
    font_button = NULL;
    font_block = NULL;
}

void draw_text(SDL_Renderer* ren, const char* text, int x, int y, int width, int height, SDL_Color color, TTF_Font* font) {

    if (!strcmp(text, "-1")) {
        return;
    }

    if (!font) {
        init_fonts();
        if (!font) return;
    }

    SDL_Surface* surf = TTF_RenderText_Blended(font, text, color);
    if (!surf) return;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    if (!tex) {
        SDL_FreeSurface(surf);
        return;
    }

    int new_x, new_y;
    int text_width, text_height;
    TTF_SizeText(font, text, &text_width, &text_height);

    // Исправленная формула центрирования
    new_x = x + (width - text_width) / 2;
    new_y = y + (height - text_height) / 2;

    SDL_Rect dst = { new_x, new_y, surf->w, surf->h };
    SDL_RenderCopy(ren, tex, NULL, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
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
    printf("Drawing '%s' focused=%d bg=%d,%d,%d text=%d,%d,%d\n",
        button.name, focused,
        c.r, c.g, c.b,
        tc.r, tc.g, tc.b);
    draw_text(ren, button.name, button.position.x, button.position.y, default_width, default_height, tc, font_button);

}

void draw_block(SDL_Renderer* ren, Block block) {
    if (block.health == 0) {
        return;
    }

    SDL_Color c = block.color;

    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);

    SDL_Rect blockRect =
    {
        block.position.x,
        block.position.y,
        block.size.width,
        block.size.height,
    };

    SDL_RenderFillRect(ren, &blockRect);

    char s_health[4];
    sprintf_s(s_health,sizeof(s_health), "%d", block.health);
    SDL_Color c_b;
    c_b.r = 0;
    c_b.g = 0;
    c_b.b = 0;

    draw_text(ren, s_health, block.position.x, block.position.y, block.size.width, block.size.height, c_b, font_block);
}


void draw_ball(SDL_Renderer* ren, Ball ball) {
    SDL_Color c = ball.color;
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);

    int r = BALL_RADIUS;
    int x = ball.position.x;
    int y = ball.position.y;

    for (int i = -r; i <= r; i++) {
        int new_x = x + i;
        int new_y;

        for (int j = -r; j <= r; j++) {
            new_y = y + j;
            if ((i * i + j * j) <= r * r) {
                //printf("%d %d\n", new_x, new_y);
                SDL_RenderDrawPoint(ren, new_x, new_y);
            }

        }
    }

}

void draw_buttons(SDL_Renderer* ren, ButtonList buttons, int focused, SDL_Color c) {
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

void draw_blocks(SDL_Renderer* ren, BlockList blocks) {
    for (int i = 0; i < blocks.count; i++) {

        draw_block(ren, blocks.block_list[i]);
    }

    draw_block(ren, blocks.vaus);
}

void draw_balls(SDL_Renderer* ren, BallList balls) {
    for (int i = 0; i < balls.count; i++) {

        draw_ball(ren, balls.balls_list[i]);
    }
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


void draw_lives(SDL_Renderer* ren, int lives_left) {
    //SDL_Surface* image = SDL_LoadBMP("./heart.bmp");
    SDL_Surface* image = SDL_LoadBMP("C:/Users/danil/PycharmProjects/pythonProject/venv/codes/help/meow/heart.bmp");
    if (!image) {
        printf("Error of loading Image meow\n");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, image);
    SDL_FreeSurface(image); 

    for (int i = 0; i < lives_left; i++) {
        SDL_Rect destRect;

        int img_width = 30, img_height = 30;
        int interval = 10;
        destRect.x = WINDOW_WIDTH - (i+1) * (img_width + interval);
        destRect.y = WINDOW_HEIGHT - interval*5;
        destRect.w = img_width;
        destRect.h = img_height;

        SDL_RenderCopy(ren, texture, NULL, &destRect);
    }

    SDL_DestroyTexture(texture);

}


void draw_blocks_and_balls(SDL_Renderer* ren, BlockList blocks, BallList balls, SDL_Color c, int lives_left) {
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, 255);
    SDL_RenderClear(ren);

    draw_lives(ren, lives_left);
    draw_blocks(ren, blocks);
    draw_balls(ren, balls);

    SDL_RenderPresent(ren);
}