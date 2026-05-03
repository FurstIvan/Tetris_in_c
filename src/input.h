#ifndef TETRIS_INPUT_H
#define TETRIS_INPUT_H

#include "types.h"

Input_types input_mapper(const bool*);
void input_handler(Input_types, Tetris*);

#endif