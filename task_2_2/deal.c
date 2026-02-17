#include <SDL.h>
#include <math.h>
#include "header.h"




static Point current_pos;
static double current_angle;
static SDL_Renderer* global_ren;

static void draw_line_recursive(SDL_Renderer* ren, Point* pos, double* angle, double length) {

    // угол привести к диапазону от 0 до 360
    double norm_angle = *angle;
    while (norm_angle >= 360.0) norm_angle -= 360.0;
    while (norm_angle < 0.0) norm_angle += 360.0;
    
    Point end;
    double rad = norm_angle * 3.14159265358979323846 / 180.0;
    end.x = pos->x + length * cos(rad);
    end.y = pos->y + length * sin(rad);

    draw_line_double(ren, *pos, end);
    pos->x = end.x;
    pos->y = end.y;
}
static void turn(double* angle, int delta) {
    *angle += delta;
    while (*angle >= 360.0) *angle -= 360.0;
    while (*angle < 0.0) *angle += 360.0;
}

static void curve(SDL_Renderer* ren, Point* pos, double* angle, unsigned order, double length, int turn_angle) {
    if (length < 0.1) {
        return;
    }
    
    if (order == 0) {
        draw_line_recursive(ren, pos, angle, length);
    }
    else {
        curve(ren, pos, angle, order - 1, length / 2.0, -turn_angle);
        turn(angle, turn_angle);
        curve(ren, pos, angle, order - 1, length / 2.0, turn_angle);
        turn(angle, turn_angle); 
        curve(ren, pos, angle, order - 1, length / 2.0, -turn_angle);
    }
}

void sierpinski_arrowhead_curve(SDL_Renderer* ren, Point start, Point end, double base_length, unsigned order) {
    if (order > MAX_ITERATIONS) {
        order = MAX_ITERATIONS;
    }
    
    Point pos = start;
    
    double dx = end.x - start.x;
    double dy = end.y - start.y;
    // без этой формулы кривая только вправо двигалась бы, а так атан вычисляет угол 
    // между вектором и OX и делает значение angle нужным для curve
    double angle = atan2(dy, dx) * 180.0 / M_PI;  
    while (angle >= 360.0) angle -= 360.0;
    while (angle < 0.0) angle += 360.0;

    if ((order % 2) == 0) {
        curve(ren, &pos, &angle, order, base_length, +60);
    }
    else {
        turn(&angle, -60);
        curve(ren, &pos, &angle, order, base_length, +60);
    }
}


 //https://en.wikipedia.org/wiki/Sierpiński_curve
// Sierpiński curve wiki (en)


void set_new_triangle(Point start, Point end) {
    array.now_iter = 0;
    Line start_line = { start, end };
    array.iterations[0].lines[0] = start_line;
    array.iterations[0].count = 1;
}

//void make_new_iter(int iter) {
//    for (int i = 0; i < array.iterations[iter - 1].count; i++) {
//        Line el = array.iterations[iter - 1].lines[i];
//        int sx = el.start.x, sy = el.start.y, ex = el.end.x, ey = el.end.y;
//        if (abs(sy-ey) < 10) {
//            return;
//        }
//
//    }
//}





//void go_to_iter(int d_iter) {
//    int iter = array.now_iter;
//    if (d_iter == 1) {
//        if (iter >= MAX_ITERATIONS) {
//            printf("no way too hard for me");
//            return;
//        }
//        iter++;
//    }
//    else if (d_iter == -1) {
//        if (iter > 0) {
//            iter--;
//        }
//        else {
//            printf("wtf man we dont have -1 curve");
//            return;
//        }
//    }
//
//    if (array.iterations[iter].count) {
//        array.now_iter = iter;
//    }
//    else {
//        make_new_iter(iter);
//        array.now_iter = iter;
//    }
//}

void go_to_iter(int d_iter) {
    int iter = array.now_iter + d_iter;
    if (iter >= 0 && iter < MAX_ITERATIONS) {
        array.now_iter = iter;
    }
}
