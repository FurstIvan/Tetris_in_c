cmake_minimum_required(VERSION 3.20)

project(tetris C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)

add_executable(tetris
    main.c
    assets.c
    render.c
    board.c
    collision.c
    data.c
    game.c
    input.c
    queue.c
    render.c
    tetris_main_loop.c
    tetrimino.c
)

target_include_directories(tetris PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(tetris PRIVATE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)

add_custom_command(TARGET tetris POST_BUILD
    COMMAND cmake -E copy_directory ../assets assets
)