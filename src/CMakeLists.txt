cmake_minimum_required(VERSION 3.20)

project(tetris_automated C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)
find_package(OpenMP REQUIRED)



add_executable(tetris_automated
    main.c
    assets.c
    render.c
    board.c
    collision.c
    data.c
    game.c
    input.c
    queue.c
    tetris_main_loop.c
    tetrimino.c
    calculate_score.c
    )

set(ALGO_SPEED "0.0" CACHE STRING "delay between algo steps for visibility")
target_compile_definitions(tetris_automated PRIVATE ALGO_SPEED=${ALGO_SPEED})

target_include_directories(tetris_automated PRIVATE ${CMAKE_SOURCE_DIR}/../include)

target_link_libraries(tetris_automated PRIVATE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    OpenMP::OpenMP_C
    )

add_custom_command(TARGET tetris_automated POST_BUILD
    COMMAND cmake -E copy_directory ../assets assets
    )