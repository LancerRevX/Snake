#pragma once

#include <stdbool.h>

#include "sdl.h"

void snake_init(Point start_point, size_t units_count);
void snake_set_speed(double speed);
void snake_set_angular_speed(double angular_speed);
void snake_set_head_texture(SDL_Texture* texture);
void snake_set_body_texture(SDL_Texture* texture);

double snake_get_angle();
double snake_get_angular_speed();
size_t snake_get_length();

void snake_move(double dt);
void snake_turn_left(double dt);
void snake_turn_right(double dt);

bool snake_out_of_bounds(int screen_width, int screen_height);
bool snake_head_overlaps_body();

bool snake_head_overlaps_circle(Point center, double radius);

void snake_grow(size_t units_count);

void snake_draw();
