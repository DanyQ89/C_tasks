#include "header.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

BallList reset_balls(BlockList* blocks) {
    BallList balls = { 0 };
    Ball ball = { 0 };
    ball.color.r = 255;
    ball.color.g = 255;
    ball.color.b = 255;

    ball.position.x = blocks->vaus.position.x + blocks->vaus.size.width / 2;
    ball.position.y = blocks->vaus.position.y - BALL_RADIUS;

    ball.velocity.x = 0;
    ball.velocity.y = 0;

    balls.count++;
    balls.balls_list[0] = ball;
    return balls;
}

void set_new_game(SDL_Renderer* ren, BlockList* blocks, BallList* balls, SDL_Color c) {
    Block vaus;

    int w = VAUS_WIDTH, h = VAUS_HEIGHT;

    vaus.color.r = 143;
    vaus.color.g = 254;
    vaus.color.b = 9;

    vaus.health = -1;


    vaus.size.width = w;
    vaus.size.height = h;

    vaus.position.x = WINDOW_WIDTH / 2 - w / 2;
    vaus.position.y = WINDOW_HEIGHT - h * 5;

    blocks->vaus = vaus;

    memset(balls->balls_list, 0, sizeof(balls->balls_list));

    *balls = reset_balls(blocks);

    draw_blocks_and_balls(ren, *blocks, *balls, c, LIVES_HAVE);
}

void launch_balls(BallList* balls) {

    int speed = BALL_SPEED;
    int angle = BALL_ANGLE;

    for (int i = 0; i < balls->count; i++) {
    
        // Система координат повернута на 90 градусов относительно обычной, поэтому синус и косинус меняются местами
        balls->balls_list[i].velocity.x = -(int)(speed * sin(angle)); // - чтобы летел вправо
        balls->balls_list[i].velocity.y = (int)(speed * cos(angle));
        //printf("%d %d", balls->balls_list[i].velocity.x, balls->balls_list[i].velocity.y);

    }
}

void move_balls(BallList* balls) {
    
    for (int i = 0; i < balls->count; i++) {
        //printf("%d %d\n", balls->balls_list[i].velocity.x, balls->balls_list[i].velocity.y);

        balls->balls_list[i].position.x += balls->balls_list[i].velocity.x / 4;
        balls->balls_list[i].position.y += balls->balls_list[i].velocity.y / 4;
    }

}


void remove_balls(BallList* balls, BlockList* blocks, bool* ball_released, int* lives_left) {
    int new_cnt = 0;

    for (int i = 0; i < balls->count; i++) {
        if (balls->balls_list[i].position.y - BALL_RADIUS <= WINDOW_HEIGHT) {
            if (i != new_cnt) {
                balls->balls_list[new_cnt] = balls->balls_list[i];
            }
            new_cnt++;
        }
    }

    for (int i = new_cnt; i < balls->count; i++) {
        // &balls потому что передаем указатель который нужно очистить, а не значение 
        memset(&balls->balls_list[i], 0, sizeof(Ball));
    }

    balls->count = new_cnt;
    
    if (new_cnt == 0) {
        if (*lives_left == 1) {
            *lives_left = 0;
            *balls = reset_balls(blocks);
            *ball_released = false;

            return;
        }

        *lives_left -= 1;

        *balls = reset_balls(blocks);
        *ball_released = false;
    }

}

void check_balls_collision(BallList* balls, BlockList* blocks) {
    int r = BALL_RADIUS;
    int vaus_x = blocks->vaus.position.x,
        vaus_y = blocks->vaus.position.y,
        vaus_w = blocks->vaus.size.width,
        vaus_h = blocks->vaus.size.height;

    int vaus_left = vaus_x;
    int vaus_right = vaus_left + vaus_w;
    int vaus_top = vaus_y;
    int vaus_bottom = vaus_top + vaus_h;

    for (int i = 0; i < balls->count; i++) {
        Ball ball_now = balls->balls_list[i];

        int ball_speed_now = (int)sqrt(ball_now.velocity.x * ball_now.velocity.x
            + ball_now.velocity.y * ball_now.velocity.y) + 1;

        int ball_x = balls->balls_list[i].position.x;
        int ball_y = balls->balls_list[i].position.y;

        int ball_right = ball_x + r;
        int ball_left = ball_x - r;
        int ball_top = ball_y - r;
        int ball_bottom = ball_y + r;



        // left and right wall
        if ((ball_left <= 0) || (ball_right >= WINDOW_WIDTH)) {
            balls->balls_list[i].velocity.x *= -1;
            continue;
        }
        // upper wall
        if (ball_top <= 0) {
            balls->balls_list[i].velocity.y *= -1;
            continue;
        }
        // vaus
        if (ball_bottom >= vaus_top && ball_top <= vaus_bottom &&
            ball_right >= vaus_left && ball_left <= vaus_right) {

            balls->balls_list[i].velocity.y *= -1;

            int center_diff = ball_x - (vaus_left + vaus_w / 2);
            
            balls->balls_list[i].velocity.x += center_diff / 10;
            continue;

        }

        // blocks
        for (int j = 0; j < blocks->count; j++) {
            Block now_block = blocks->block_list[j];
            if (!now_block.health) {
                continue;
            }
            int block_left = now_block.position.x;
            int block_right = now_block.position.x + now_block.size.width;
            int block_top = now_block.position.y;
            int block_bottom = now_block.position.y + now_block.size.height;


            // left and right side collision

            if (((abs(ball_right - block_left) <= ball_speed_now) || (abs(ball_left - block_right) <= ball_speed_now)) &&
                (
                    (ball_bottom >= block_top)
                    && 
                    (ball_top <= block_bottom)
                    )) {

                balls->balls_list[i].velocity.x *= -1;
                blocks->block_list[j].health -= 1;
                continue;
            }

            // upper and lower side collision
            else if (((abs(ball_top - block_bottom) <= ball_speed_now) || (abs(ball_bottom - block_top) <= ball_speed_now)) &&
                (

                    (ball_left <= block_right)
                    &&
                    (ball_right >= block_left)
                    )) {

                balls->balls_list[i].velocity.y *= -1;
                blocks->block_list[j].health -= 1;
                continue;
            }
        }
    }
}

bool have_blocks(BlockList* blocks) {
    for (int i = 0; i < blocks->count; i++) {
        if (blocks->block_list[i].health) {
            return true;
        }
    }

    return false;
}

char* update_game(BallList* balls, BlockList* blocks, bool* ball_released, int* lives_left) {
    if (!have_blocks(blocks)) {
        return "next level";
    }
    if (*ball_released) {
        for (int step = 0; step < STEPS_CHECK; step++) {  // 4 микро-шага за кадр
            move_balls(balls);  
            check_balls_collision(balls, blocks);  
        }
    }
    remove_balls(balls, blocks, ball_released, lives_left);
    if (*lives_left == 0) {
        return "game over";
    }
    return "ok";
}