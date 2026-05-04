#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include "types.h"

Tetris* init_game();
void get_new_tetrimino(Tetris*);
void place_tetrimino(Tetris*, int, int);
void game_over_check(Tetris* tetris);
void place_clear_over_new(Tetris*);
void free_tetris(Tetris*);
void update(Tetris*);
void hard_drop(Tetris*);
void hold(Tetris*);

#endif