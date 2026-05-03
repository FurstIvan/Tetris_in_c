#ifndef TETRIS_COLLISION_H
#define TETRIS_COLLISION_H

#include "types.h"

bool fall_collision(Tetris*, int);
bool side_collision(Tetris*, int);
bool rotation_collision(Tetris*, Tetrimino_State);

#endif