#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window* window;
SDL_Renderer* renderer;

enum {
    SCREEN_WIDTH = 640,
    SCREEN_HEIGHT = 480,
};

typedef struct Point Point;
struct Point {
    double x;
    double y;
};

void sdl_init();

SDL_Texture* load_texture(char const* path);

void draw_circle(Point center, double radius);

void sdl_destroy();

#endif // SDL_H
