#include "board.h"
#include "data.h"
#include "game.h"
#include "tetrimino.h"

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


bool fall_collision_sim(Board* board,Tetrimino* moving_tetrimino ,int next_top_row) {
    int height = board -> height;

    Tetrimino copy_tetrimino = *(moving_tetrimino);
    Tetrimino* copy_tetrimino_ptr = &(copy_tetrimino);
    copy_tetrimino_ptr -> height_top = next_top_row;

    const Board* locked = board;
    const Rotation_mask* rotation = &(all_shape_Tetrimino_State_Map[copy_tetrimino_ptr ->shape][copy_tetrimino_ptr ->tetrimino_state]);

    if (locked ->height - rotation -> height - copy_tetrimino_ptr ->height_top < 0) {
        return true;
    }

    for (int row = 0; row < TETRIMINO_BOX_HEIGHT; row++) {
        uint16_t after_falling = get_current_piece_row(copy_tetrimino_ptr, row);
        int board_row = next_top_row + row;

        if (after_falling == 0) {continue;}

        if ((locked -> arr[board_row] & after_falling) != 0) {
            return true;
        }
    } 

    return false;
}


void place_tetrimino_sim(Board* board,Tetrimino* tetrimino ,uint8_t height_top, uint8_t left) {
    Tetrimino_State state = tetrimino -> tetrimino_state;

    for (int i = 0; i < TETRIMINO_BOX_HEIGHT; i++) {
        uint16_t row_ready = get_current_piece_row(tetrimino, i);

        board -> arr[height_top + i] |= row_ready;
    }
    return;
}


void calculate_hard_Drop_Board(Tetris* tetris) { // will only conation the difference between if we hard-drop and the normal board

    for (uint8_t i = 0; i < tetris->locked_board->height; i++) {
        tetris->hard_drop_board->arr[i] = tetris->locked_board->arr[i]; 
    }

    uint8_t moving_tetrimino_height_copy = tetris->moving_tetrimino->height_top; // downcast!
    uint8_t x_right_moving_tetrimino_copy = tetris->moving_tetrimino->x_right; //downcast!

    while (!fall_collision_sim(tetris->hard_drop_board,tetris->moving_tetrimino ,moving_tetrimino_height_copy + 1)) {
        moving_tetrimino_height_copy= moving_tetrimino_height_copy + 1;
    }

    place_tetrimino_sim(tetris->hard_drop_board, tetris->moving_tetrimino, moving_tetrimino_height_copy, x_right_moving_tetrimino_copy);
    
    for (uint8_t i = 0; i < tetris->locked_board->height; i++) {
        tetris->hard_drop_board->arr[i] = tetris->hard_drop_board->arr[i] - tetris->locked_board->arr[i] ; 
    }
        

    return;
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
