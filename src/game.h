#ifndef GAME_H
#define GAME_H

#include "tetris.h"

Tetris* init_game(void);
void game_loop(Tetris* tetris);
bool collision(const Tetris* tetris, int next_top_row);

#endif