#ifndef TETRIS_TYPES_H
#define TETRIS_TYPES_H

#include "common.h"
#include "assets.h"

typedef struct Tetrimino Tetrimino;
typedef struct Queue Queue;
typedef struct Tetris Tetris;
typedef struct Board Board;
typedef struct Input Input;
typedef struct Tetrimino_Color Tetrimino_Color;
typedef struct Rotation_mask Rotation_mask;

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
    RAMP0 = 100,
    RAMP1 = 95,
    RAMP2 = 85,
    RAMP3 = 75,
    RAMP4 = 55,
    RAMP5 = 45,
    RAMP6 = 40,
    RAMP7 = 30,
    RAMP8 = 20,
    RAMP9 = 15,
    END   = 10
}Gravity;

typedef enum {
    MOV_LEFT,
    MOV_RIGHT,
    MOV_DOWN_SLOW,
    DROP_DOWN,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    HOLD,
    TRAP,
    QUIT
} Input_types;

struct Input{
    Input_types input_type;
    bool is_pressed;
    bool is_released;
} ;

struct Board{            // board width is 16 but im only using 10 ig
    uint16_t* arr;
    int height;
    int width;
} ;

struct Tetrimino_Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct Tetrimino{
    int height_top;            // where the top part of the tetrimino is
    int x_right;               // the right side of the tetrimino x
    Tetrimino_State tetrimino_state;
    Shape shape;
} ;

struct Tetris{
    Gravity grav;
    Gamestate gs;
    Board* locked_board;
    Tetrimino* moving_tetrimino;
    Queue* tetrimino_shape_queue;
    uint64_t score;
    double input_buffer_speed;
    Tetrimino hold_tetrimino;
    bool played_hold;
    Assets* asset;
} ; 

struct Queue{
    Shape shapes[SHAPE_QUEUE_LEN];
    int front;
    int rear;
    int lenght;
    int size;
} ;

struct Rotation_mask {
    uint8_t rows[4];
    uint8_t width;
    int height;
} ;

#endif