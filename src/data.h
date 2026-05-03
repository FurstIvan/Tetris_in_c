#ifndef TETRIS_DATA_H
#define TETRIS_DATA_H

#include "types.h"

extern Gravity grav_ramp_states[];
extern uint8_t simple_score_board[];
extern Tetrimino_Color color_map[t_shape + 2];
extern const Rotation_mask all_shape_Tetrimino_State_Map[t_shape + 1][Rotated_thrice +1];

#endif