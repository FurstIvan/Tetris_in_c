cmake_minimum_required(VERSION 3.10)

project(tetris_main C)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

add_executable(tetris_main main.c data.c sdl_app.c render.c input.c game.c collision.c tetrimino.c board.c queue.c)

target_include_directories(tetris_main PRIVATE src)
target_link_libraries(tetris_main PRIVATE SDL3::SDL3)