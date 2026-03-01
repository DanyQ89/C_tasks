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

//void get_filename(char* buf, int size) {
//    FILE* file = NULL;
//
//    do {
//        printf("Введите путь файла:\n");
//        fgets(buf, size, stdin);
//        buf[strcspn(buf, "\n")] = 0;
//
//        if (buf[0] == '\0') {
//            printf("Пустой ввод, попробуйте ещё раз\n");
//            continue;
//        }
//
//        file = fopen(buf, "r");
//        if (!file) {
//            printf("Файл не открывается");
//            continue;
//        }
//    } while (!file);
//
//    fclose(file);
//}



char* SystemUpdate(SDL_Window* window, SDL_Renderer* ren, Menu menu) {

    //printf("%s", menu.buttons_list.buttons[2].name);

    bool running = true;
    SDL_Event event = { 0 };
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    char filename[256];
    int now_index = 0;

    make_window(window, ren, menu.window);


    ButtonsList buttons = menu.buttons_list;
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
                case DOWN:
                    printf("%d\n", now_index);

                    if (now_index == lenn - 1) {
                        now_index = 0;

                    }
                    else {
                        now_index++;
                    }
                    draw_buttons(ren, buttons, now_index, menu.window.color);                        break;
                case UP:
                    printf("%d\n", now_index);

                    if (now_index == 0) {
                        now_index = lenn - 1;
                    }
                    else {
                        now_index--;
                    }
                    draw_buttons(ren, buttons, now_index, menu.window.color);                        break;
                case ENTER:
                    return buttons.buttons[now_index].name;
                case QUIT:
                    running = false;
                    break;
                }
            }

    }

        //Command_pressed cmd = get_command(keystate);


        //if (cmd.quit) {
        //    running = false;
        //}
        //
        //if (cmd.enter) {
        //    //printf("Была нажата кнопка '%s'", buttons.buttons[now_index].name);
        //    return buttons.buttons[now_index].name;
        }
}