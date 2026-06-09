#ifndef CALCULATE_SCORE_H
#define CALCULATE_SCORE_H

#include "common.h"
#include "data.h"
#include "game.h"
#include "tetrimino.h"
#include "board.h"
#include "types.h"
#include <omp.h>

#define BOARD_EMPTY_ROW_VALUE 0
#define start_of_board 0

typedef struct  
{
    Board* board;
    uint32_t score_sim;
    uint8_t lines_cleared;
} Board_n_score;

#define MAX_LINK VISIBLE_WIDTH * (Rotated_thrice + 1)

typedef struct tetris_move {
    Shape shape;
    uint8_t x_right;
    Tetrimino_State rotation;
} tetris_move;

typedef struct Board_Tree_Node {
    Board_n_score this_node;

    struct Board_Tree_Node* parent;
    struct Board_Tree_Node* link_Nodes[MAX_LINK];
    uint8_t link_count;

    uint8_t tree_depth;
    bool expanded; // has child or no 
    bool valid; // is the score of the board okay or no

    tetris_move move;

    uint32_t best_score;
    struct Board_Tree_Node* best_link;

} Board_Tree_Node;

typedef struct Board_tree {
    Board_Tree_Node* trunk;
} Board_tree;

typedef struct Board_tree_queue {
    Board_Tree_Node** queue_of_tree_nodes;
    uint32_t end;
    uint32_t beg;
} Board_tree_queue;

void delete_tree_recursive(Board_Tree_Node* );
Board_tree* create_board_tree_from_current_board(const Board*, const Queue*);
void extend_board_tree_to_full_depth (Board_tree*, const Queue* );
bool promote_best_child_to_root(Board_tree* );
void recalculcate_depths(Board_Tree_Node*, uint8_t );
void copy_from_board_to_board (Board* , const Board*) ;

#endif