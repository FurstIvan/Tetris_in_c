#ifndef TETRIS_H
#define TETRIS_H

#include <stdint.h>
#include <stdbool.h>

#define TARGET_FPS 60.0
#define FIXED_STEP (1.0 / 60.0)
#define MAX_FRAME_TIME 0.25

#define HEIGHT 20
#define WIDTH 10

#define ROTATION_RIGHT do {                                                                                                      \
        tetris->moving_tetrimino->tetrimino_state = (tetris->moving_tetrimino->tetrimino_state + 1) % 4;                       \
    } while (0)

#define ROTATION_LEFT do {                                                                                                       \
        tetris->moving_tetrimino->tetrimino_state = (tetris->moving_tetrimino->tetrimino_state + 3) % 4;                       \
    } while (0)

typedef enum {
    INGAME,
    LOST,
    WON
} Gamestate;

typedef enum {
    Neutral,
    Rotated_once,
    Rotated_twice,
    Rotated_thrice
} Tetrimino_State;

typedef enum {
    left_l_shape,
    right_l_shape,
    square,
    I_block,
    left_z_block,
    right_z_block,
    t_shape
} Shape;

typedef enum {
    BEGINNING = 1,
    END = 10
} Gravity;

typedef enum {
    MOV_LEFT,
    MOV_RIGHT,
    MOV_DOWN_SLOW,
    DROP_DOWN,
    ROTATE_LEFT,
    ROTATE_RIGHT
} Inputs;

typedef struct {
    uint16_t* arr;
    int height;
    int width;
} Board;

typedef struct {
    int height_top;
    int left_x;
    uint16_t bitmask[4];
    Tetrimino_State tetrimino_state;
    Shape shape;
} Tetrimino;

typedef struct {
    Gravity grav;
    Gamestate gs;
    Board* locked_board;
    Tetrimino* moving_tetrimino;
} Tetris;

#endif