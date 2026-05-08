#include "tetrimino.h"
#include "data.h"

Tetrimino* init_tetrimino(const Shape shape) {
    Tetrimino* tetrimino = calloc(1, sizeof(Tetrimino));

    tetrimino -> shape = shape;
    tetrimino -> height_top = 0;
    tetrimino -> x_right = 3;
    tetrimino -> tetrimino_state = Neutral;

    return tetrimino;
}

Tetrimino set_tetrimino(
    Shape shape,
    uint8_t x_right,
    uint8_t height_top,
    Tetrimino_State tetrimino_state) {
    Tetrimino tetrimino = {.tetrimino_state = tetrimino_state, .shape = shape, .x_right = x_right, .height_top = height_top};

    return tetrimino;
}


void free_tetrimino(Tetrimino* tetrimino) {
    free(tetrimino);
}

void rotate_right(Tetrimino* t) {
    t -> tetrimino_state = (t -> tetrimino_state + 1) % 4;
}
void rotate_left(Tetrimino* t) {
    t -> tetrimino_state = (t -> tetrimino_state + 3) % 4;
}

uint16_t get_current_piece_row(Tetrimino* tetrimino, int row) {
    const Rotation_mask* rotation = &(all_shape_Tetrimino_State_Map[tetrimino -> shape][tetrimino ->tetrimino_state]);

    uint16_t raw_row = (uint16_t)rotation -> rows[row];

    if (raw_row == 0) {
        return 0;
    }

    // we get the shift by knowing the width of the tetrimino (2) - where the tetrimino is (3)
    uint8_t shift = VISIBLE_WIDTH - rotation -> width - tetrimino -> x_right; 

    return raw_row << shift;
}