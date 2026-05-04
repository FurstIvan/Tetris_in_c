#include "board.h"
#include "data.h"

Board* init_board(int height) {
    Board* board = calloc(1, sizeof(Board));
    board -> arr = calloc(1, sizeof(uint16_t) * height);
    board -> height = height; 
    board -> width  = VISIBLE_WIDTH + HIDDEN_WIDTH_SIZE;

    for (int i = 0; i < height; i++) {
        board -> arr[i] = 0;
    } 

    return board;
}

void free_board(Board* board) {
    free(board -> arr);
    free(board);
    return;
}

void print_board(Tetris* tetris) {
    int h = tetris -> locked_board -> height;
    uint16_t* board_arr = tetris -> locked_board -> arr;

    for (int i = 0; i < h; i++) {
        uint16_t value_of_the_board_space = *(board_arr + i);

        for (int j = VISIBLE_WIDTH - 1; j >= 0; j--){
            if ( (value_of_the_board_space & (1 << j)) != 0 ) {
                printf("X");
            }
            else {
                printf("_");
            }
        }
        printf("%u\n", value_of_the_board_space);
    }

    return;
}

void line_clear(Board* board, int row) {
    board -> arr[row] = 0;
    for (; row > 1; row--) {
        board -> arr[row] = board -> arr[row - 1];
    }
}

static bool last_clear_tetris = false;

void line_clear_check(Tetris* tetris) {
    int line_clear_mask = (1u <<  VISIBLE_WIDTH) - 1;

    uint8_t lines_cleared = 0;
    for (int row = tetris->locked_board->height - 1; row >= 2; row--) {
        if (tetris -> locked_board -> arr[row] == line_clear_mask) {
            line_clear(tetris->locked_board, row);
            lines_cleared += 1;
            row += 1;
        }
    }
    uint16_t score_to_add = simple_score_board[lines_cleared] * 100;
    if (last_clear_tetris) {score_to_add *= 1.5;}
    if (lines_cleared > 3) {last_clear_tetris = true;}

    tetris -> score += score_to_add;
}