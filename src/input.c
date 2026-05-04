#include "input.h"
#include "collision.h"
#include "game.h"

void input_handler(Input_types input_conv, Tetris* tetris) {
    switch (input_conv) {
        case MOV_LEFT:{
            
            int next_right_x = tetris->moving_tetrimino->x_right - 1;
            if (side_collision(tetris, next_right_x)) {
            } else {
                tetris->moving_tetrimino->x_right = next_right_x;
            }
            break;
        }

        case MOV_RIGHT:{ 
            int next_right_x = tetris->moving_tetrimino->x_right + 1;
            if (side_collision(tetris, next_right_x)) {
            } else {
                tetris->moving_tetrimino->x_right = next_right_x;
            }
            break;
        }

        case MOV_DOWN_SLOW:{
            int next_height = tetris->moving_tetrimino->height_top+1;
            if (fall_collision(tetris,next_height)) {
                place_clear_over_new(tetris);
            } else {
                tetris->moving_tetrimino->height_top = next_height;
            }
            break;
        }

        case DROP_DOWN:{
            hard_drop(tetris);
            break;
        }

        case ROTATE_LEFT:{
            if ( rotation_collision(tetris, ROTATION_LEFT) ){
                break;
            }
            ACTUAL_ROTATION_LEFT;
            break;
        }

        case ROTATE_RIGHT:{
            if ( rotation_collision(tetris, ROTATION_RIGHT) ){
                break;
            }
            ACTUAL_ROTATION_RIGHT;
            break;
        }

        case QUIT:{
            tetris->gs = LOST;
            break;
        }

        case HOLD: {
            if (tetris -> played_hold) {
            } else {
                hold(tetris);
                tetris -> played_hold = true;
            }
            break;
        }

        case TRAP:{
            break;
        }

        default: {
            break;
        }
    }
    return;
}

Input_types input_mapper(const bool *keys) {
    if (keys[SDL_SCANCODE_A]) {
        return MOV_LEFT;
    }

    if (keys[SDL_SCANCODE_D]) {
        return MOV_RIGHT;
    }

    if (keys[SDL_SCANCODE_S]) {
        return MOV_DOWN_SLOW;
    }

    if (keys[SDL_SCANCODE_SPACE]) {
        return DROP_DOWN;
    }

    if (keys[SDL_SCANCODE_W]) {
        return ROTATE_LEFT;
    }

    if (keys[SDL_SCANCODE_E]) {
        return ROTATE_RIGHT;
    }

    if (keys[SDL_SCANCODE_Q]) {
        return QUIT;
    }

    if (keys[SDL_SCANCODE_H]) {
        return HOLD;
    }
    return TRAP;
}