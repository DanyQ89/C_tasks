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
#define COLOR_RENDER_R 251
#define COLOR_RENDER_G 24
#define COLOR_RENDER_B 104
#define COLOR_RENDER_A 255
#define opacity 40
#define rasst_move 5.0 

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} App;

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point now_pos;
    Point last_pos;
    double now_diameter;
    bool moved;
} Point_Update;

enum {
    W = SDL_SCANCODE_W,
    A = SDL_SCANCODE_A,
    S = SDL_SCANCODE_S,
    D = SDL_SCANCODE_D,
    LEFT = SDL_SCANCODE_LEFT,
    UP = SDL_SCANCODE_UP,
    DOWN = SDL_SCANCODE_DOWN,
    RIGHT = SDL_SCANCODE_RIGHT,
    O = SDL_SCANCODE_O,
    ENTER = SDL_SCANCODE_RETURN
};


typedef struct {
    bool up, down, right, left, quit, reset;
} Command_pressed;

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

        //pp(ren, new_x, y);

        for (int j = -r; j <= r; j++) {
            new_y = y + j;


            if ((i * i + j * j) <= r * r) {
                //printf("%d %d", new_x, new_y);
                pp(ren, new_x, new_y);
            }
            //pp(ren, x + i, y + j);


        }

    }


}

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

    // создает для ускорения работы окна и показывает его
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    //Uint32 render_flags = SDL_RENDERER_PRESENTVSYNC;

    app.renderer = SDL_CreateRenderer(app.window, -1,
        render_flags);

    return app;
}

Command_pressed get_command(const Uint8* keystate) {
    Command_pressed cmd = { 0 };
    cmd.up = keystate[W] || keystate[UP];
    cmd.down = keystate[S] || keystate[DOWN];
    cmd.left = keystate[A] || keystate[LEFT];
    cmd.right = keystate[D] || keystate[RIGHT];
    cmd.reset = keystate[O];
    cmd.quit = keystate[ENTER];
    return cmd;
}



Point_Update update_point(Point current_pos, double current_diameter, Command_pressed cmd) {
    Point_Update update = {
        .now_pos = current_pos,
        .last_pos = current_pos,
        .now_diameter = current_diameter + 0.9,
        .moved = false
    };

    bool horizontal_move = cmd.right || cmd.left;
    bool vertical_move = cmd.up || cmd.down;
    bool is_diagonal = horizontal_move && vertical_move;
    update.moved = horizontal_move || vertical_move;

    if (!update.moved) {
        return update;
    }

    double minus = update.now_diameter / 2.0;

    if (cmd.up) {
        update.last_pos.y += (is_diagonal) ? minus / sqrt(2) : minus;
        update.now_pos.y -= rasst_move;
    }
    if (cmd.down) {
        update.last_pos.y -= (is_diagonal) ? minus / sqrt(2) : minus;
        update.now_pos.y += rasst_move;
    }
    if (cmd.right) {
        update.last_pos.x -= (is_diagonal) ? minus / sqrt(2) : minus;
        update.now_pos.x += rasst_move;
    }
    if (cmd.left) {
        update.last_pos.x += (is_diagonal) ? minus / sqrt(2) : minus;
        update.now_pos.x -= rasst_move;
    }

    if (cmd.reset) {
        update.now_diameter = diameter;
    }

    return update;
}

int main() {
    SDL_Color color_circle = { .r = COLOR_CIRCLE_R, .g = COLOR_CIRCLE_G, .b = COLOR_CIRCLE_B, .a = COLOR_CIRCLE_A };
    //SDL_Color color_render = { .r = 120,.g = 20,.b = 147,.a = 0 };
    SDL_Color color_render = { .r = COLOR_RENDER_R, .g = COLOR_RENDER_G, .b = COLOR_RENDER_B, .a = COLOR_RENDER_A };

    App app = init();
    if (app.window == NULL || app.renderer == NULL) {
        return 1;
    }

    SDL_Renderer* ren = app.renderer;
    draw_first(ren, color_render);
    Point now_pos = { 100.0, 100.0 };
    double now_d = diameter;

    draw_w_point(ren, now_pos.x, now_pos.y, diameter, color_circle);
    SDL_RenderPresent(ren);

    bool running = true;
    SDL_Event event;
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        Command_pressed cmd = get_command(keystate);

        if (cmd.quit) {
            running = false;
        }
        else if (cmd.reset) {
            now_d = diameter;
        }

        Point_Update update = update_point(now_pos, now_d, cmd);

        if (update.moved) {
            now_pos = update.now_pos;
            now_d = update.now_diameter;

            // след делаем шире
            draw_w_point(ren, update.last_pos.x, update.last_pos.y, now_d, color_circle);
            make_it_darker(ren, color_render);

            // наст эл
            draw_w_point(ren, now_pos.x, now_pos.y, diameter, color_circle);
            SDL_RenderPresent(ren);
        }

        SDL_Delay(10);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
    return 0;
}
