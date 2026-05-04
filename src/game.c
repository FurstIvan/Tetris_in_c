#include "game.h"
#include "queue.h"
#include "board.h"
#include "tetrimino.h"
#include "collision.h"

Tetris* init_game(){
    Gravity grav = 0;
    int height = VISIBLE_HEIGHT + HIDDEN_LAYER_SIZE;
    int width  = VISIBLE_WIDTH + HIDDEN_WIDTH_SIZE;  
    Gamestate gs = INGAME;

    Tetris* tetris = malloc(sizeof(Tetris));
    tetris -> tetrimino_shape_queue = malloc(sizeof(Queue));
    init_queue(tetris -> tetrimino_shape_queue);

    tetris -> grav = grav;
    tetris -> gs = gs;
    tetris -> locked_board = init_board(height);
    tetris -> score = 0;
    tetris -> input_buffer_speed = INPUT_BUFFER_SPEED;
    tetris -> hold_tetrimino = (Tetrimino) {.height_top= -100};
    tetris -> played_hold = false;
    tetris -> asset = font_init();

    srand((time(NULL)));
    Shape shape = rand() % (t_shape + 1);

    tetris -> moving_tetrimino = init_tetrimino(shape);
    Queue* que_ptr = tetris -> tetrimino_shape_queue;

    for (int i = 0; i < SHAPE_QUEUE_LEN; i++) {
        Shape random_shape = rand() % (t_shape + 1);
        enqueue(que_ptr, random_shape);
    }
    print_queue(que_ptr);

    return tetris;
}

void update(Tetris* tetris) {
    int next_top = tetris->moving_tetrimino->height_top+1;
    if (fall_collision(tetris, next_top)) {
        place_clear_over_new(tetris);
    } else {
        tetris->moving_tetrimino->height_top = next_top; 
    }
}

void hold(Tetris* tetris) {
    if (tetris -> hold_tetrimino.height_top == -100) {
        tetris -> hold_tetrimino = *(tetris -> moving_tetrimino);

        tetris -> hold_tetrimino.tetrimino_state = Neutral;
        tetris -> hold_tetrimino.height_top = 0;
        tetris -> hold_tetrimino.x_right = 3;
        get_new_tetrimino(tetris);
    } else {
        Tetrimino temp = tetris -> hold_tetrimino; 
        tetris -> hold_tetrimino = *(tetris ->moving_tetrimino); 
        *(tetris -> moving_tetrimino) = temp;

        tetris -> hold_tetrimino.tetrimino_state = Neutral;
        tetris -> hold_tetrimino.height_top = 0;
        tetris -> hold_tetrimino.x_right = 3;
    }

    return;
}

void place_tetrimino(Tetris* tetris, int height_top, int left) {
    Tetrimino* tetrimino = tetris -> moving_tetrimino;
    Tetrimino_State state = tetrimino -> tetrimino_state;
    Board* board = tetris ->locked_board;

    for (int i = 0; i < TETRIMINO_BOX_HEIGHT; i++) {
        uint16_t row_ready = get_current_piece_row(tetrimino, i);

        board -> arr[height_top + i] |= row_ready; 
    }
    return;
}

void get_new_tetrimino(Tetris* tetris) {
    Tetrimino* tetrimino = tetris -> moving_tetrimino;
    tetrimino -> shape = dequeue(tetris -> tetrimino_shape_queue);

    if (tetrimino -> shape == -1) {
        tetris -> gs = LOST; 
        return;
    }

    tetrimino -> height_top = 0;
    tetrimino -> x_right = 5; 
    tetrimino -> tetrimino_state = Neutral;

    Shape random_shape = rand() % (t_shape + 1);
    enqueue(tetris -> tetrimino_shape_queue, random_shape);

    return;
}

void place_clear_over_new(Tetris* tetris) {
    int new_height_top = tetris->moving_tetrimino->height_top;
    tetris ->played_hold = false;
    place_tetrimino(tetris, new_height_top, tetris->moving_tetrimino -> x_right);
    line_clear_check(tetris);
    game_over_check(tetris);
    get_new_tetrimino(tetris);
}

void hard_drop(Tetris* tetris) {
    while (!fall_collision(tetris, tetris -> moving_tetrimino -> height_top + 1)) {
        tetris -> moving_tetrimino ->height_top = tetris -> moving_tetrimino ->height_top + 1;
    }
    place_clear_over_new(tetris);
    return;
}

void game_over_check(Tetris* tetris) {
    Tetrimino* tetrimino = tetris -> moving_tetrimino;
    if (tetrimino -> height_top <= 2) {
        tetris -> gs = LOST;
        return;
    }
    tetris -> gs = INGAME;
    return;
}

void free_tetris(Tetris* tetris) {
    
    Queue* q = tetris ->tetrimino_shape_queue;
    Board* board = tetris -> locked_board;
    Tetrimino* moving_tetrimino = tetris -> moving_tetrimino;
    Assets* asset = tetris -> asset;

    free_queue(q);
    free_board(board);
    free_tetrimino(moving_tetrimino);
    font_free(asset);
    free(tetris);
    return;
}