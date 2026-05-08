#ifndef TETRIS_TETRIMINO_H
#define TETRIS_TETRIMINO_H

#include "types.h"

Tetrimino* init_tetrimino(Shape);
void rotate_right(Tetrimino*);
void rotate_left(Tetrimino*);
uint16_t get_current_piece_row(Tetrimino*, int);
void free_tetrimino(Tetrimino*);
Tetrimino set_tetrimino(Shape, uint8_t, uint8_t, Tetrimino_State);

#endif