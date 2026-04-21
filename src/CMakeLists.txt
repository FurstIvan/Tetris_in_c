cmake_minimum_required(VERSION 3.10)

project(tetris_main C)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

add_executable(tetris_main tetris_main.c)
target_link_libraries(tetris_main PRIVATE SDL3::SDL3)