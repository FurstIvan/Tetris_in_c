#include "game.h"

#include <stdlib.h>
#include <time.h>

#include <SDL3/SDL.h>

#include "board.h"
#include "tetrimino.h"

Tetris* init_game(void) {
    Gravity grav = BEGINNING;
    int height = HEIGHT + 2;
    int width = 16;
    Gamestate gs = INGAME;

    Tetris* tetris = malloc(sizeof(Tetris));
    tetris->grav = grav;
    tetris->gs = gs;
    tetris->locked_board = init_board(height);

    srand((time(NULL)));
    Shape shape = rand() % (t_shape + 1);

    tetris->moving_tetrimino = init_tetrimino(shape);

    return tetris;
}

/// to be done: drop down hard
void game_loop(Tetris* tetris) {
    bool running = true;
    while (running) {
        SDL_Event input;
        switch (tetris->gs) {
            case INGAME:
                while (SDL_PollEvent(&input)) {
                    switch (input.type) {
                        case MOV_LEFT:
                            tetris->moving_tetrimino->left_x++;
                            if (collision(tetris, tetris->moving_tetrimino->height_top))
                                tetris->moving_tetrimino->left_x--;
                            break;

                        case MOV_RIGHT:
                            tetris->moving_tetrimino->left_x--;
                            if (collision(tetris, tetris->moving_tetrimino->height_top))
                                tetris->moving_tetrimino->left_x++;
                            break;

                        case MOV_DOWN_SLOW:
                            tetris->moving_tetrimino->height_top--;
                            if (collision(tetris, tetris->moving_tetrimino->height_top))
                                tetris->moving_tetrimino->height_top++;
                            break;

                        case DROP_DOWN:
                            break;

                        case ROTATE_LEFT:
                            ROTATION_LEFT;

                            if (collision(tetris, tetris->moving_tetrimino->height_top))
                                ROTATION_RIGHT;
                            break;

                        case ROTATE_RIGHT:
                            ROTATION_RIGHT;

                            if (collision(tetris, tetris->moving_tetrimino->height_top))
                                ROTATION_LEFT;
                            break;
                    }

                    print_board(tetris);
                    int i = 0;
                    for (; !collision(tetris, tetris->moving_tetrimino->height_top); i++) {
                        Board locked_board_copy = *(tetris->locked_board);

                        locked_board_copy.arr[i] = tetris->moving_tetrimino->bitmask[i];
                        locked_board_copy.arr[i + 1] = tetris->moving_tetrimino->bitmask[i + 1];
                    }
                    tetris->locked_board->arr[i] += tetris->moving_tetrimino->bitmask[i];
                    tetris->locked_board->arr[i + 1] += tetris->moving_tetrimino->bitmask[i + 1];
                }
                break;

            case LOST:
                running = false;
                break;

            case WON:
                running = false;
                break;

            default:
                running = false;
        }
    }
}

bool collision(const Tetris* tetris, int next_top_row) {
    int h = tetris->locked_board->height;
    int w = tetris->locked_board->width;
    Tetrimino* moving_tetrimino = tetris->moving_tetrimino;
    const Board* locked = tetris->locked_board;

    if (moving_tetrimino->left_x == 10 || moving_tetrimino->left_x - 4 == 0)
        return true;

    for (int r = 0; r < 4; r++) {
        uint16_t piece_row = moving_tetrimino->bitmask[r];

        int board_row = next_top_row + r;

        if (board_row >= locked->height) {
            return true;
        }

        if ((locked->arr[board_row] & piece_row) != 0) {
            return true;
        }
    }

    return false;
}