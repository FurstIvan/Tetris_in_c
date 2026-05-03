#include "collision.h"
#include "data.h"
#include "tetrimino.h"

bool fall_collision(Tetris* tetris, int next_top_row) {
    int height = tetris -> locked_board -> height;

    Tetrimino copy_tetrimino = *(tetris -> moving_tetrimino);
    Tetrimino* copy_tetrimino_ptr = &(copy_tetrimino);
    copy_tetrimino_ptr -> height_top = next_top_row;

    const Board* locked = tetris -> locked_board;
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

bool side_collision(Tetris* tetris, int next_right_x) {
    Tetrimino moving_tetrimino_copy = *(tetris -> moving_tetrimino);
    moving_tetrimino_copy.x_right = next_right_x;

    Tetrimino* moving_tetrimino_copy_ptr = &(moving_tetrimino_copy);

    const Board* locked = tetris -> locked_board;
    const Rotation_mask* rotation = &(all_shape_Tetrimino_State_Map[moving_tetrimino_copy_ptr ->shape][moving_tetrimino_copy_ptr ->tetrimino_state]);

    if (next_right_x < 0) {
        return true;
    }
    if (next_right_x + rotation -> width > VISIBLE_WIDTH) {
        return true;
    }

    for (int row = 0; row < TETRIMINO_BOX_HEIGHT; row++) {
        uint16_t after_moving = get_current_piece_row(moving_tetrimino_copy_ptr, row);
        int board_row = moving_tetrimino_copy_ptr -> height_top + row;

        if (after_moving == 0) {
            continue;
        }

        if ((locked -> arr[board_row] & after_moving) != 0) {
            return true;
        }
    } 

    return false;
}

bool rotation_collision(Tetris* tetris, Tetrimino_State next_rotation) {
    Tetrimino moving_tetrimino_copy = *(tetris->moving_tetrimino);
    moving_tetrimino_copy.tetrimino_state = next_rotation;

    Tetrimino* tetrimino_copy_ptr = &(moving_tetrimino_copy);

    const Board* locked = tetris -> locked_board;
    const Rotation_mask* rotation = &(all_shape_Tetrimino_State_Map[tetrimino_copy_ptr ->shape][tetrimino_copy_ptr -> tetrimino_state]);

    if ( locked -> height < (rotation -> height + tetrimino_copy_ptr -> height_top) ) {
        return true;
    }
    if ( VISIBLE_WIDTH - tetrimino_copy_ptr -> x_right - rotation -> width < 0) {
        return true;
    }
    if ( tetrimino_copy_ptr -> x_right < 0) {
        return true;
    }

    for (int row = 0; row < TETRIMINO_BOX_HEIGHT; row++) {
        uint16_t after_moving = get_current_piece_row( tetrimino_copy_ptr, row);
        int board_row = tetrimino_copy_ptr -> height_top + row;

        if (after_moving == 0) {
            continue;
        }

        if (board_row < 0 || board_row >= locked -> height) {
            return true;
        }

        if ((locked->arr[board_row] & after_moving) != 0) {
            return true;
        }
    }

    return false;
}