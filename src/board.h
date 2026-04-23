#ifndef BOARD_H
#define BOARD_H

#include "tetris.h"

Board* init_board(int height);
void print_board(Tetris* tetris);
void lock_piece(Tetris* tetris);

#endif