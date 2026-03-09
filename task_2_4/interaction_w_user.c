#include <SDL.h>
#include "header.h"


Command_pressed get_command(const Uint8* keystate) {
    Command_pressed cmd = { 0 };
    cmd.left = keystate[LEFT];
    cmd.right = keystate[RIGHT];
    cmd.up = keystate[UP];
    cmd.down = keystate[DOWN];
    cmd.enter = keystate[ENTER];
    cmd.quit = keystate[QUIT];
    return cmd;
}





void SystemUpdate(SDL_Window* window, SDL_Renderer* ren, Menu menu) {

    //printf("%s", menu.buttons_list.buttons[2].name);

    bool running = true;
    SDL_Event event = { 0 };
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    char filename[256];
    int now_index_button = -1;
    char* pressed_button;
    BlockList blocks = { 0 };
    BallList balls = { 0 };
    bool in_game = false;
    bool ball_released = false;
    int vaus_speed = VAUS_SPEED;
    int lives_left = LIVES_HAVE;
    int now_level = 0;

    LevelList levels = { .count = 2 };
    levels.levels[0] = "level_1.txt";
    levels.levels[1] = "level_2.txt";

    make_window(window, ren, menu.window);


    ButtonList buttons = menu.buttons_list;
    size_t lenn = buttons.count;
    if (!lenn && lenn > 0) {
        return;
    }

    draw_buttons(ren, buttons, 0, menu.window.color);

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.scancode) {


                case SPACE:
                    printf("%d", ball_released);
                    if (!in_game || ball_released) {
                        break;
                    }
                    launch_balls(&balls);
                    ball_released = true;
                    break;
                case DOWN:
                    in_game = false;
                    ball_released = false;

                    if (now_index_button == lenn - 1) {
                        now_index_button = 0;

                    }
                    else {
                        now_index_button++;
                    }
                    draw_buttons(ren, buttons, now_index_button, menu.window.color); 
                    break;
                case UP:
                    in_game = false;
                    ball_released = false;

                    if (now_index_button == 0) {
                        now_index_button = lenn - 1;
                    }
                    else {
                        now_index_button--;
                    }
                    draw_buttons(ren, buttons, now_index_button, menu.window.color);
                    break;
                case ENTER:
                    if (in_game) {
                        break;
                    }

                    pressed_button = buttons.buttons[now_index_button].name;
                    if (!strcmp(pressed_button, "Start Game")) {

                        in_game = true;
                        blocks = LevelInitialize("./level_1.txt");

                        set_new_game(ren, &blocks, &balls, menu.window.color);


                    }
                    else if (!strcmp(pressed_button, "Exit")) {
                        return;
                    }
                    break;
                case QUIT:
                    running = false;
                    break;
                }
            }

        }

       

        Command_pressed cmd = get_command(keystate);


        if (cmd.right) {
            if (in_game) {
                if (blocks.vaus.position.x + vaus_speed + VAUS_WIDTH <= WINDOW_WIDTH) {
                    blocks.vaus.position.x += vaus_speed;

                    if (!ball_released) {
                        for (int i = 0; i < balls.count; i++) {
                            balls.balls_list[i].position.x += vaus_speed;
                        }
                }
                }
            }
        }
        if (cmd.left) {
            if (in_game) {

                if (blocks.vaus.position.x - vaus_speed >= 0) {

                    if (!ball_released) {
                        for (int i = 0; i < balls.count; i++) {
                            balls.balls_list[i].position.x -= vaus_speed;
                        }
                    }

                    blocks.vaus.position.x -= vaus_speed;

                }

            }
        }
        
        if (in_game) {
            if (ball_released) {
                
                char* result = update_game(&balls, &blocks, &ball_released, &lives_left);

                if (!strcmp(result, "game over")) {
                    blocks = LevelInitialize("./level_1.txt");
                    set_new_game(ren, &blocks, &balls, menu.window.color);
                    ball_released = false;
                    lives_left = LIVES_HAVE;
                    now_level = 0;
                    in_game = false;
                }
                else if (!strcmp(result, "next level")) {
                    if (now_level + 1 < levels.count) {
                        now_level++;
                        blocks = LevelInitialize(levels.levels[now_level]);
                        set_new_game(ren, &blocks, &balls, menu.window.color);
                        ball_released = false;
                        lives_left = LIVES_HAVE;
                    }
                    else {
                        in_game = false;
                        ball_released = false;
                        now_index_button = 0;

                        draw_buttons(ren, buttons, now_index_button, menu.window.color);
                    }
                }
            }
            draw_blocks_and_balls(ren, blocks, balls, menu.window.color, lives_left);
        }

        SDL_Delay(5);
    }
}