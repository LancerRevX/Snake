#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sdl.h"

#include "snake.h"

struct {
    SDL_Texture* texture;
    SDL_Rect rect;
} apple;

int main() {
    srand(time(0));

    sdl_init();

    apple.texture = load_texture("images/apple.png");
    apple.rect.w = 32;
    apple.rect.h = 32;
    apple.rect.x = rand() % (SCREEN_WIDTH - apple.rect.w) + apple.rect.w;
    apple.rect.y = rand() % (SCREEN_HEIGHT - apple.rect.h) + apple.rect.h;

    snake_init((Point){200.0, SCREEN_HEIGHT - 32}, 4);
    snake_set_angular_speed(3.0);
    snake_set_head_texture(load_texture("images/head.png"));
    snake_set_body_texture(load_texture("images/body.png"));

    enum {
        NONE, LEFT, RIGHT
    } moving = NONE;
    double speed = 64;
    snake_set_speed(speed);

    Uint8 const* key_pressed = SDL_GetKeyboardState(0);

    double time = 0.0;
    double dt = 0.0;
    bool running = true;
    bool game_over = false;
    while (running) {
            Uint32 start_time = SDL_GetTicks();

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT: {
                        running = false;
                        break;
                    }
                    case SDL_KEYDOWN: {
                        switch (event.key.keysym.scancode) {
#ifndef NDEBUG
                            case SDL_SCANCODE_RETURN: {
                                if (game_over)
                                    snake_init((Point){200.0, SCREEN_HEIGHT - 32}, 4);
                                    game_over = false;
                                break;
                            }
#endif
                            case SDL_SCANCODE_SPACE: {
                                snake_grow(4);
                                break;
                            }
                        }
                        break;
                    }
                }
            }


            if (!game_over) {
                if (key_pressed[SDL_SCANCODE_LEFT]) {
                    if (moving == NONE)
                        moving = LEFT;
                    snake_turn_left(dt);
                } else if (moving == LEFT) {
                    moving = NONE;
                }

                if (key_pressed[SDL_SCANCODE_RIGHT]) {
                    if (moving == NONE)
                        moving = RIGHT;
                    snake_turn_right(dt);
                } else if (moving == RIGHT) {
                    moving = NONE;
                }

                if (key_pressed[SDL_SCANCODE_UP]) {
                    snake_set_speed(speed * 2.0);
                } else if (key_pressed[SDL_SCANCODE_DOWN]) {
                    snake_set_speed(speed / 2.0);
                } else {
                    snake_set_speed(speed);
                }

                snake_move(dt);

                if (snake_out_of_bounds(SCREEN_WIDTH, SCREEN_HEIGHT) ||
                    snake_head_overlaps_body()) {
                    game_over = true;
                    char title[64];
                    sprintf(title, "Game over! Your score: %lu. Press enter to continue...", snake_get_length());
                    SDL_SetWindowTitle(window, title);
                    continue;
                }

                if (snake_head_overlaps_circle(
                        (Point) {
                            apple.rect.x + apple.rect.w / 2.0,
                            apple.rect.y + apple.rect.w / 2.0
                        },
                        apple.rect.w / 2.0
                    )) {
                    snake_grow(4);
                    apple.rect.x = rand() % (SCREEN_WIDTH - apple.rect.w) + apple.rect.w;
                    apple.rect.y = rand() % (SCREEN_HEIGHT - apple.rect.h) + apple.rect.h;
                }



                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
                SDL_RenderClear(renderer);

                snake_draw();

                SDL_RenderCopy(
                    renderer,
                    apple.texture,
                    0,
                    &apple.rect
                );

//                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
//                draw_circle((Point) {
//                                apple.rect.x + apple.rect.w / 2.0,
//                                apple.rect.y + apple.rect.w / 2.0
//                            }, apple.rect.w / 2.0);

                SDL_RenderPresent(renderer);

                char title[24];
                sprintf(title, "Length: %lu", snake_get_length());
                SDL_SetWindowTitle(window, title);

                time += dt;
                if (time >= 1.0)
                    time = 0.0;
                dt = (SDL_GetTicks() - start_time) / 1000.0;
            }
    }

    sdl_destroy();
    return EXIT_SUCCESS;
}
