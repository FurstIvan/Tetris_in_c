#ifndef TETRIS_COMMON_H
#define TETRIS_COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#define TARGET_FPS 60.0
#define TARGET_FRAME_TIME (1.0 / TARGET_FPS)
#define MAX_FRAME_TIME 0.1

#define INPUT_BUFFER_SPEED 12

%:define something <% char name <:4:> = "Bob"; %>;

#define ACTUAL_ROTATION_RIGHT tetris -> moving_tetrimino -> tetrimino_state = (tetris -> moving_tetrimino -> tetrimino_state + 1) % 4;                
#define ROTATION_RIGHT (tetris -> moving_tetrimino -> tetrimino_state + 1) % 4  

#define ACTUAL_ROTATION_LEFT tetris -> moving_tetrimino -> tetrimino_state = (tetris -> moving_tetrimino -> tetrimino_state + 3) % 4;
#define ROTATION_LEFT (tetris -> moving_tetrimino -> tetrimino_state + 3) % 4

#define SCORE_NEED_FOR_GRAV_RAMPING_UP 2000

#define VISIBLE_HEIGHT 20
#define VISIBLE_WIDTH 10

#define HIDDEN_LAYER_SIZE 2
#define HIDDEN_WIDTH_SIZE 6

#define LOGICAL_WIDTH 480
#define LOGICAL_HEIGHT 640

#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH 640

#define BLOCK_SIZE 28

#define BOARD_X ( LOGICAL_WIDTH - BOARD_W ) / 2
#define BOARD_Y ( LOGICAL_HEIGHT - BOARD_H ) / 2

#define BOARD_W (VISIBLE_WIDTH * BLOCK_SIZE) 
#define BOARD_H (VISIBLE_HEIGHT * BLOCK_SIZE)

#define DEFAULT_COLOR 255,255,255

#define TETRIMINO_BOX_HEIGHT 4

#define NEXT_TETRIMINO_ARRAY_X BOARD_X + (10 * BLOCK_SIZE) + 20
#define NEXT_TETRIMINO_ARRAY_Y BOARD_Y 
#define TETRIMINO_ARRAY_BLOCK_SIZE BLOCK_SIZE / TETRIMINO_BOX_HEIGHT //we need to make each tetrimino 4 times smaller so it fits

#define HOLD_TETRIMINO_X BOARD_X - (1 * BLOCK_SIZE) - 20
#define HOLD_TETRIMINO_Y BOARD_Y 

#define TEXT_WIDTH_PIXEL (float)50
#define TEXT_HEIGHT_PIXEL (float)25

#define SCORE_FONT_SIZE 20.0f

#define SCORE_TEXT_X LOGICAL_WIDTH / 2 
#define SCORE_TEXT_Y BOARD_Y - 50

// macros in the future #define TYPE_OF_INT uint_ ## (HIDDEN_WIDTH + VISIBLE_WIDTH)

// think as in an array of row masks

#endif

#ifndef ALGO_SPEED
#define ALGO_SPEED 0.1
#endif

#ifndef SHAPE_QUEUE_LEN
#define SHAPE_QUEUE_LEN 2
#endif

#ifndef PARALLEL_ON
#define PARALLEL_ON 1
#endif

#ifdef FUTURE_SIGHT
#if FUTURE_SIGHT > SHAPE_QUEUE_LEN
#undef FUTURE_SIGHT
#define FUTURE_SIGHT SHAPE_QUEUE_LEN
#endif
#endif

#ifndef FUTURE_SIGHT
#define FUTURE_SIGHT SHAPE_QUEUE_LEN
#endif

#ifndef AMOUNT_SHAPE
#define AMOUNT_SHAPE 4
#endif