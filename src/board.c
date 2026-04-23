#include "board.h"

#include <stdio.h>
#include <stdlib.h>

Board* init_board(int height) {
    Board* board = calloc(1, sizeof(Board));
    board->arr = calloc(1, sizeof(uint16_t) * height);
    board->height = height;
    board->width = 16;

    for (int i = 0; i < height; i++) {
        board->arr[i] = 0;
    }

    return board;
}

void print_board(Tetris* tetris) {
    int h = tetris->locked_board->height;
    int w = tetris->locked_board->width;
    uint16_t* board_arr = tetris->locked_board->arr;

    for (int i = 0; i < h; i++) {
        int value_of_the_board_space = *(board_arr + i);

        for (int j = w - 1; j > 0; j--) {
            if ((value_of_the_board_space & (1 << j)) != 0) {
                printf("X");
            } else {
                printf("_");
            }
        }
        printf("%d\n", value_of_the_board_space);
    }

    return;
}

void lock_piece(Tetris* tetris) {
    Board* locked = tetris->locked_board;
    Tetrimino* moving_tetrimino = tetris->moving_tetrimino;

    for (int r = 0; r < 4; r++) {
        uint16_t piece_row = moving_tetrimino->bitmask[r];

        if (piece_row == 0) continue;

        int board_row = moving_tetrimino->height_top + r;

        if (board_row >= 0 && board_row < locked->height)
            locked->arr[board_row] |= piece_row;
    }
}