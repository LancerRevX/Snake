#include "snake.h"

#include <math.h>

#include <stdio.h>

SDL_Texture* head;
SDL_Texture* body;

double speed = 0.0;
double angular_speed = 0.0;

double unit_radius = 16.0;

double acceleration = 1.0;
size_t length;
#define max_units_count 128
Point units[max_units_count];
#define path_length 4096
Point path[path_length];
double angle;

void snake_init(Point start_point, size_t units_count)
{
    length = units_count;
    double y = start_point.y;
    for (size_t i = 0; i < path_length; i++) {
        path[i].x = start_point.x;
        path[i].y = y;
        y++;
    }
    for (size_t i = 0; i < max_units_count; i++) {
        units[i].x = -100;
        units[i].y = -100;
    }
    angle = M_PI / 2;
}

void snake_set_speed(double _speed)
{
    speed = _speed;
}

void snake_set_angular_speed(double _angular_speed)
{
    angular_speed = _angular_speed;
}

void snake_set_head_texture(SDL_Texture *texture)
{
    head = texture;
}

void snake_set_body_texture(SDL_Texture *texture)
{
    body = texture;
}

void snake_move(double dt)
{
    for (size_t i = path_length - 1; i > 0; i--) {
        path[i] = path[i - 1];
    }

    Point point = path[0];
    point.x += cos(angle) * acceleration * speed * dt;
    point.y -= sin(angle) * acceleration * speed * dt;
    path[0] = point;

    Point last_point = path[0];
    for (size_t i = 1, j = 0; i < path_length && j < length - 1; i++) {
        if (sqrt(pow(path[i].x - last_point.x, 2) + pow(path[i].y - last_point.y, 2)) >= unit_radius) {
            units[j].x = path[i].x;
            units[j].y = path[i].y;
            last_point = units[j];
            j++;
        }
    }
}

void snake_turn_left(double dt)
{
    angle += angular_speed * dt;
}

void snake_turn_right(double dt)
{
    angle -= angular_speed * dt;
}

bool snake_out_of_bounds(int screen_width, int screen_height) {
    if (path[0].x - unit_radius < 0 ||
        path[0].y - unit_radius < 0 ||
        path[0].x + unit_radius > screen_width ||
        path[0].y + unit_radius > screen_height)
        return true;
    else
        return false;
}

bool snake_head_overlaps_body() {
    for (size_t i = 2; i < length - 1; i++) {
        if (sqrt(pow(units[i].x - path[0].x, 2) + pow(units[i].y - path[0].y, 2)) < unit_radius * 2.0)
            return true;
    }
    return false;
}

bool snake_head_overlaps_circle(Point center, double radius) {
    if (sqrt(pow(center.x - path[0].x, 2) + pow(center.y - path[0].y, 2)) < radius + unit_radius)
        return true;
    else
        return false;
}

void snake_grow(size_t units_count) {
    length += units_count;
}

void snake_draw() {
#ifndef NDEBUG
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    for (size_t i = 0; i < path_length; i++) {
        SDL_RenderDrawPoint(renderer, path[i].x, path[i].y);
    }
#endif
    SDL_Rect rect = {
        .x = path[0].x - unit_radius,
        .y = path[0].y - unit_radius,
        .w = unit_radius * 2.0,
        .h = unit_radius * 2.0,
    };
    SDL_RenderCopyEx(
        renderer,
        head,
        0,
        &rect,
        -angle / M_PI * 180.0 + 90.0,
        0,
        0
    );
    for (size_t i = 0; i < length - 1; i++) {
        rect.x = units[i].x - unit_radius;
        rect.y = units[i].y - unit_radius;
        SDL_RenderCopy(
            renderer,
            body,
            0,
            &rect
        );
    }
}

double snake_get_angular_speed() {
    return angular_speed;
}

double snake_get_angle() {
    return angle;
}

size_t snake_get_length() {
    return length;
}
