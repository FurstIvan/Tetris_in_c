#ifndef TETRIS_BOARD_H
#define TETRIS_BOARD_H

#include "types.h"

void print_board(Tetris*);
Board* init_board(int);
void line_clear_check(Tetris*); 
void line_clear(Board*, int);
void free_board(Board*);

#endif