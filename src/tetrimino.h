#ifndef TETRIMINO_H
#define TETRIMINO_H

#include "tetris.h"

static const uint16_t all_shape_Tetrimino_State_Map[t_shape + 1][4] = {
    {
        4,
        30244,
        17126,
        512
    },
    {
        2,
        18146,
        29734,
        1024
    },
    {
        26214,
        26214,
        0,
        0
    },
    {
        514,
        62194,
        514,
        514
    },
    {
        16448,
        25443,
        9766,
        0
    },
    {
        8224,
        27756,
        17990,
        0
    },
    {
        0,
        18210,
        25191,
        16416
    }
};

Tetrimino* init_tetrimino(Shape shape);
void rotate_right(Tetrimino* t);
void rotate_left(Tetrimino* t);

#endif