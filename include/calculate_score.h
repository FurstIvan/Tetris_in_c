#ifndef CALCULATE_SCORE_H
#define CALCULATE_SCORE_H

#include "common.h"
#include "data.h"
#include "game.h"
#include "tetrimino.h"
#include "board.h"
#include <omp.h>

#define BOARD_EMPTY_ROW_VALUE 0
#define start_of_board 0

typedef struct  
{
    Board* board;
    uint32_t score_sim;
} Board_n_score;

Board* simulate_drop_hard_and_place(const Board*, const Tetrimino*);
Board_n_score* score_board_given_shape_at_pos_rotation(const Board*, const Shape, const uint8_t, uint8_t, Tetrimino_State);
Board* find_best_board_for_shape_omp(const Board*, Shape);
bool apply_best_board_for_shape_omp(Board*, Shape);
void free_board_ptr(Board*);

#endif