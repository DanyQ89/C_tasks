#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#define diameter 30
#define width 500
#define height 500
#define COLOR_CIRCLE_R 120
#define COLOR_CIRCLE_G 219
#define COLOR_CIRCLE_B 226
#define COLOR_CIRCLE_A 0
#define COLOR_RENDER_R 0
#define COLOR_RENDER_G 0
#define COLOR_RENDER_B 0
#define COLOR_RENDER_A 255
#define opacity 40

typedef struct {
    double x;
    double y;
} Point;

enum {
    W = SDL_SCANCODE_W,
    S = SDL_SCANCODE_S,
    A = SDL_SCANCODE_A,
    D = SDL_SCANCODE_D,
    UP = SDL_SCANCODE_UP,
    DOWN = SDL_SCANCODE_DOWN,
    LEFT = SDL_SCANCODE_LEFT,
    RIGHT = SDL_SCANCODE_RIGHT,
    O = SDL_SCANCODE_O,
    ENTER = SDL_SCANCODE_RETURN
};

SDL_Color color_circle = { .r = COLOR_CIRCLE_R, .g = COLOR_CIRCLE_G, .b = COLOR_CIRCLE_B, .a = COLOR_CIRCLE_A };
//SDL_Color color_render = { .r = 120,.g = 20,.b = 147,.a = 0 };
SDL_Color color_render = { .r = COLOR_RENDER_R, .g = COLOR_RENDER_G, .b = COLOR_RENDER_B, .a = COLOR_RENDER_A };



//void less_brightness(SDL_Renderer* ren, int x, int y, float factor) {
//    int r = diameter / 2;
//
//    for (int x = x - r; x <= x + r; x++) {
//        for (int y = y - r; y <= y + r; y++) {
//
//
//        }
//    }
//}


void draw_first(SDL_Renderer* ren, SDL_Color color) {
    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
    SDL_RenderClear(ren);
}


void pp(SDL_Renderer* ren, int x, int y) {
    SDL_RenderDrawPoint(ren, x, y);
}


void make_it_darker(SDL_Renderer* ren, SDL_Color color) {
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, opacity);
    //SDL_SetRenderDrawColor(ren, 0, 0, 0, 10);

    SDL_Rect rect = { 0, 0, width, height };
    SDL_RenderFillRect(ren, &rect);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);

}

void draw_w_point(SDL_Renderer* ren, int x, int y, int d, SDL_Color color) {
    //make_it_darker(ren, color_render);

    int r = d / 2;

    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);



    for (int i = -r; i <= r; i++) {
        int new_x = x + i;
        int new_y;

        pp(ren, new_x, y);

        for (int j = -r; j <= r; j++) {
            new_y = y + j;


            if ((i * i + j * j) <= r * r) {
                //printf("%d %d", new_x, new_y);
                pp(ren, new_x, new_y);
            }
            //pp(ren, x + i, y + j);


        }

    }

    SDL_RenderPresent(ren);

}

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} App;

App init() {
    App app = { NULL, NULL };

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        return app;
    }

    // окно

    app.window = SDL_CreateWindow(
        "Moving point",
        //SDL_WINDOWPOS_CENTERED,
        //SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        0
    );



    //color.g = 89;
    //color.b = 69;
    //color.a = 0;


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

    SDL_Window* window = app.window;
    SDL_Renderer* ren = app.renderer;


    draw_first(ren, color_render);
    Point now_pos = { 100.0, 100.0 };
    double now_d = diameter;

    double now_x = now_pos.x;
    double now_y = now_pos.y;

    draw_w_point(ren, now_x, now_y, diameter, color_circle);
    SDL_RenderPresent(ren);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) { // любое событие
            if (event.type == SDL_QUIT) running = false; // крестик
            if (event.type == SDL_KEYDOWN) { // если любая нажата
                if (SDL_GetModState() & KMOD_CTRL) { // и ctrl (сложно чисто отдельно)
                    running = false;
                }
                if (event.key.keysym.scancode == ENTER) { // если ретурн просто нажали
                    running = false;
                }

            }


            const Uint8* keystate = SDL_GetKeyboardState(NULL);

            bool moved = false;

            now_d += 0.9;
            double minus = now_d / 2.0;
            Point last_pos = now_pos;

            double last_x = last_pos.x;
            double last_y = last_pos.y;

            double rasst = 5.0;
            if (keystate[O]) {
                now_d = diameter;
            }

            // Добавляем флаги для отслеживания состояния
            bool cmd_up = false;
            bool cmd_down = false;
            bool cmd_left = false;
            bool cmd_right = false;

            // Обновляем флаги на основе текущих нажатий
            if (keystate[W] || keystate[UP]) cmd_up = true;
            if (keystate[S] || keystate[DOWN]) cmd_down = true;
            if (keystate[A] || keystate[LEFT]) cmd_left = true;
            if (keystate[D] || keystate[RIGHT]) cmd_right = true;

            // Теперь определяем диагональ
            bool horizontal_move = (cmd_right || cmd_left);
            bool vertical_move = (cmd_up || cmd_down);
            bool is_diagonal = horizontal_move && vertical_move;

            // Обрабатываем движение с учетом диагонали
            if (keystate[W] || keystate[UP]) {
                if (is_diagonal) {
                    last_y += minus / 2;
                }
                else {
                    last_y += minus;
                }
                now_y -= rasst;
                moved = true;
            }
            if (keystate[S] || keystate[DOWN]) {
                if (is_diagonal) {
                    last_y -= minus / 2;
                }
                else {
                    last_y -= minus;
                }
                now_y += rasst;
                moved = true;
            }
            if (keystate[D] || keystate[RIGHT]) {
                if (is_diagonal) {
                    last_x -= minus / 2;
                }
                else {
                    last_x -= minus;
                }
                now_x += rasst;
                moved = true;
            }
            if (keystate[A] || keystate[LEFT]) {
                if (is_diagonal) {
                    last_x += minus / 2;
                }
                else {
                    last_x += minus;
                }
                now_x -= rasst;
                moved = true;
            }

            if (moved) {
                now_pos.x = now_x;
                now_pos.y = now_y;
                last_pos.x = last_x;
                last_pos.y = last_y;

                if (is_diagonal) {
                    draw_w_point(ren, last_x, last_y, now_d * 0.99, color_circle);
                }
                else {
                    draw_w_point(ren, last_x, last_y, now_d, color_circle);
                }
                make_it_darker(ren, color_render);
                draw_w_point(ren, now_x, now_y, diameter, color_circle);
                SDL_RenderPresent(ren);
            }




            //if (event.type == SDL_KEYUP) {
            //    printf("meow");
            //    draw_first(ren);

            //    now_x += 100;
            //    //now_y += 100;
            //    draw_w_point(ren, now_x, now_y, diameter, color);
            //    SDL_RenderPresent(ren);
            //}
            //SDL_Delay(1000);
            //draw_first(ren, color_render);
            //make_it_darker(ren, color_render);
            //SDL_Delay(16);


            SDL_Delay(10);

        }
    }

    // если убрать то нагружаю память проги будут тормозить из за неочистки
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
