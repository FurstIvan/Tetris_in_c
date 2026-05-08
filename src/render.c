#include "render.h"
#include "data.h"
#include "tetrimino.h"
#include "tetris_main_loop.h"
#include "assets.h"

// current frame rendering is 480, 640 -> will stretch --> 10 block wide 
// each block should be 28x28
void set_tetrimino_color(SDL_Renderer *renderer, Shape shape) {
    SDL_SetRenderDrawColor(renderer, color_map[shape].r,color_map[shape].g, color_map[shape].b, SDL_ALPHA_OPAQUE);
    return;
}

SDL_Texture* render_score_texture_creater(SDL_Renderer* renderer, Tetris* tetris, Assets* asset) {
    char score_Str[20];
    sprintf(score_Str, "%llu", tetris ->score);
    const char* score_Text = score_Str;
    SDL_Color color = (SDL_Color){.a = SDL_ALPHA_OPAQUE, .r = 0, .g = 0, .b = 0};
    SDL_Texture* texture = create_text_texture(renderer, asset -> font, score_Text, color, 0, 0);

    return texture;
}

void render_hold_tetrimino(SDL_Renderer* rernderer, Tetris* tetris) {
    SDL_FRect hold_ouline = {
        .x = HOLD_TETRIMINO_X,
        .y = HOLD_TETRIMINO_Y,
        .w = BLOCK_SIZE,
        .h = BLOCK_SIZE,
    };

    SDL_SetRenderDrawColor(rernderer, 80,80,80,SDL_ALPHA_OPAQUE);
    SDL_RenderRect(rernderer, &(hold_ouline));

    SDL_FRect block = {
        .x = 0,
        .y = 0,
        .h = TETRIMINO_ARRAY_BLOCK_SIZE,
        .w = TETRIMINO_ARRAY_BLOCK_SIZE,
    };

    if (tetris -> hold_tetrimino.height_top == UINT8_MAX) {return;}
    Shape shape = tetris -> hold_tetrimino.shape;
    const Rotation_mask* rotation = &(all_shape_Tetrimino_State_Map[shape][tetris ->hold_tetrimino.tetrimino_state]);
    
    SDL_SetRenderDrawColor(rernderer, color_map[shape].r, color_map[shape].g, color_map[shape].b, SDL_ALPHA_OPAQUE);

    int slot_height = HOLD_TETRIMINO_Y;

    for (int piece_row = 0; piece_row < TETRIMINO_BOX_HEIGHT; piece_row++) {
        uint16_t value_row = rotation -> rows[piece_row];

        if (value_row == 0) {continue;}

        for (int bit_col = 0; bit_col < TETRIMINO_BOX_HEIGHT; bit_col++) {
            if ((value_row & (1u << bit_col)) != 0) {
                int screen_col = rotation -> width - bit_col - 1;

                block.x = HOLD_TETRIMINO_X + screen_col * TETRIMINO_ARRAY_BLOCK_SIZE;
                block.y = slot_height + piece_row * TETRIMINO_ARRAY_BLOCK_SIZE;

                SDL_RenderFillRect(rernderer, &block);
            }
        }
    }


    return;
} 

/* 
board row 0, 1 = hidden
board row 2    = visible row 0
board row 21   = visible row 19

bit 9 = left side
bit 0 = right side

screen_col = 9 - bit_col
screen_x   = BOARD_X + screen_col * BLOCK_SIZE
screen_y   = BOARD_Y + visible_row * BLOCK_SIZE 
*/


void render_side_bar(SDL_Renderer* renderer, Tetris* tetris) {
    SDL_FRect side_bar_outline  = {
        .x = NEXT_TETRIMINO_ARRAY_X,
        .y = NEXT_TETRIMINO_ARRAY_Y,
        .h = BLOCK_SIZE * (SHAPE_QUEUE_LEN + 2),
        .w = BLOCK_SIZE
    };

    SDL_SetRenderDrawColor(renderer, 80, 80, 80, SDL_ALPHA_OPAQUE);
    SDL_RenderRect(renderer, &side_bar_outline);

    SDL_FRect block = {
        .h = TETRIMINO_ARRAY_BLOCK_SIZE,
        .w = TETRIMINO_ARRAY_BLOCK_SIZE,
        .x = 0,
        .y = 0
    };

    Queue* q = tetris -> tetrimino_shape_queue;

    for (int i = 0; i < q->size; i++) {
        Shape shape = q ->shapes[(q -> front + i) % q->lenght];
        const Rotation_mask* rotation = &(all_shape_Tetrimino_State_Map[shape][Neutral]);
        
        SDL_SetRenderDrawColor(renderer, color_map[shape].r, color_map[shape].g, color_map[shape].b, SDL_ALPHA_OPAQUE);

        int slot_height = (i + 1) * NEXT_TETRIMINO_ARRAY_Y + TETRIMINO_ARRAY_BLOCK_SIZE;

        for (int piece_row = 0; piece_row < TETRIMINO_BOX_HEIGHT; piece_row++) {
            uint16_t value_row = rotation -> rows[piece_row];

            if (value_row == 0) {continue;}

            for (int bit_col = 0; bit_col < TETRIMINO_BOX_HEIGHT; bit_col++) {
                if ((value_row & (1u << bit_col)) != 0) {
                    int screen_col = rotation -> width - bit_col - 1;

                    block.x = NEXT_TETRIMINO_ARRAY_X + screen_col * TETRIMINO_ARRAY_BLOCK_SIZE;
                    block.y = slot_height + piece_row * TETRIMINO_ARRAY_BLOCK_SIZE;

                    SDL_RenderFillRect(renderer, &block);
                }
            }
        }
    }
    
    return;
}

void render_current_frame(SDL_Renderer* renderer, Tetris* tetris) {
    SDL_FRect board_outline;

    board_outline.x = BOARD_X;
    board_outline.y = BOARD_Y;
    board_outline.w = BOARD_W;
    board_outline.h = BOARD_H;

    SDL_SetRenderDrawColor(renderer, 80, 80, 80, SDL_ALPHA_OPAQUE);
    SDL_RenderRect(renderer, &board_outline);

    SDL_FRect square;
    square.w = BLOCK_SIZE;
    square.h = BLOCK_SIZE;

    set_tetrimino_color(renderer, t_shape + 1);

    Board* board = tetris->locked_board;

    for (int board_row = HIDDEN_LAYER_SIZE; board_row < board->height; board_row++) {
        uint16_t value_row = board->arr[board_row];

        int screen_row = board_row - HIDDEN_LAYER_SIZE;

        if (screen_row < 0 || screen_row >= VISIBLE_HEIGHT) {
            continue;
        }

        for (int bit_col = 0; bit_col < VISIBLE_WIDTH; bit_col++) {
            if ((value_row & (1u << bit_col)) != 0) {
                int screen_col = VISIBLE_WIDTH - 1 - bit_col;

                square.x = BOARD_X + screen_col * BLOCK_SIZE;
                square.y = BOARD_Y + screen_row * BLOCK_SIZE;

                SDL_RenderFillRect(renderer, &square);
            }
        }
    }

    return;
}