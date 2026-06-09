cmake_minimum_required(VERSION 3.20)

project(tetris_automated C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)


set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
set(OpenMP_C_LIB_NAMES "omp")
set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib")


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

set(ALGO_SPEED 0.0 CACHE STRING "delay between algo steps for visibility")
set(SHAPE_QUEUE_LEN 2 CACHE STRING "how many shhapes in queue also how far the algo can see")
set(PARALLEL_ON 1 CACHE STRING "use this to use the parallelized version of the code")
set(FUTURE_SIGHT 2 CACHE STRING "how many block do you allow the algorithm to look forward into")
set(AMOUNT_SHAPE 4 CACHE STRING "lets you control the randomness" )


target_compile_definitions(tetris_automated PRIVATE ALGO_SPEED=${ALGO_SPEED} PRIVATE PARALLEL_ON=${PARALLEL_ON} PRIVATE AMOUNT_SHAPE=${AMOUNT_SHAPE} PRIVATE SHAPE_QUEUE_LEN=${SHAPE_QUEUE_LEN} PRIVATE FUTURE_SIGHT=${FUTURE_SIGHT} )

target_include_directories(tetris_automated PRIVATE ${CMAKE_SOURCE_DIR}/../include)

target_link_libraries(tetris_automated PRIVATE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    OpenMP::OpenMP_C
    )

add_custom_command(TARGET tetris_automated POST_BUILD
    COMMAND cmake -E copy_directory ../assets assets
    )