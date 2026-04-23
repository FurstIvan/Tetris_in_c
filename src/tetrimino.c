#include "tetrimino.h"

#include <stdlib.h>

Tetrimino* init_tetrimino(const Shape shape) {
    Tetrimino* tetrimino = calloc(1, sizeof(Tetrimino));
    tetrimino->shape = shape;

    for (int i = 0; i < 4; i++) {
        tetrimino->bitmask[i] |= all_shape_Tetrimino_State_Map[shape][i];
    }

    tetrimino->height_top = 0;
    tetrimino->left_x = 6;

    tetrimino->tetrimino_state = Neutral;

    return tetrimino;
}

void rotate_right(Tetrimino* t) {
    t->tetrimino_state = (t->tetrimino_state + 1) % 4;
}

void rotate_left(Tetrimino* t) {
    t->tetrimino_state = (t->tetrimino_state + 3) % 4;
}