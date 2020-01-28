cmake_minimum_required(VERSION 3.5)

project(snake LANGUAGES C)

add_executable(snake main.c snake.c snake.h sdl.h sdl.c)

target_link_libraries(snake m SDL2 SDL2main SDL2_image)
