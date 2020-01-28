#include "sdl.h"

void sdl_init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow(
        "",
        100, 100,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

SDL_Texture* load_texture(char const* path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        printf("IMG_LoadTexture Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    return texture;
}

void draw_circle(Point center, double radius) {
    for (double angle = 0.0; angle <= M_PI * 2.0; angle += 0.1) {
        double x = center.x + radius * cos(angle);
        double y = center.y + radius * sin(angle);
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

void render_texture(SDL_Texture* texture, int x, int y, int w, int h) {
    SDL_Rect rect = {
        .x = x,
        .y = y
    };
    if (w && h) {
        rect.w = w;
        rect.h = h;
    } else {
        SDL_QueryTexture(texture, 0, 0, &rect.w, &rect.h);
    }
    SDL_RenderCopy(renderer, texture, 0, &rect);
}

void sdl_destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
