#include <SDL.h>
#include "header.h"


Command_pressed get_command(const Uint8* keystate) {
    Command_pressed cmd = { 0 };
    cmd.left = keystate[A] || keystate[LEFT];
    cmd.right = keystate[D] || keystate[RIGHT];
    cmd.quit = keystate[ENTER] || keystate[SDL_SCANCODE_ESCAPE];
    cmd.reset = keystate[R];
    return cmd;
}


MouseResult handle_mouse_event(SDL_Event event, InputStateMode mode, Point start, Point end, int now_x, int now_y) {

    if (event.type == SDL_MOUSEBUTTONDOWN &&
        event.button.button == SDL_BUTTON_LEFT) {

        if (mode == WAITING_FIRST_CLICK) {
            start.x = event.button.x;
            start.y = event.button.y;
            printf("meow");
            mode = WAITING_SECOND_CLICK;
        }
        else if (mode == WAITING_SECOND_CLICK) {
            now_x = event.motion.x;
            now_y = event.motion.y;

            // при работе со списками сюда добавить проверку на делимость на 2

            end.x = now_x;
            end.y = now_y;

            set_new_triangle(start, end);

            mode = WORKING;
        }
    }

    if (event.type == SDL_MOUSEMOTION && mode == WAITING_SECOND_CLICK) {
        now_x = event.motion.x;
        now_y = event.motion.y;
        printf("%d %d %d %d\n", start.x, start.y, now_x, now_y);
        end.x = now_x;
        end.y = now_y;
    }

    MouseResult result;
    result.mode = mode;
    result.start = start;
    result.end = end;
    result.now_x = now_x;
    result.now_y = now_y;

    return result;
}