#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define FPS 60
#define HEIGHT 20
#define WIDTH 10

// think as in an array of row masks

typedef enum {
    INGAME,
    LOST,
    WON
} Gamestate;

typedef enum {
    Neutral,
    Rotated_once,
    Rotated_twice,
    Rotated_thrice
} Tetrimino_State;

typedef enum {
    left_l_shape,
    right_l_shape,
    square,
    I_block,
    left_z_block,
    right_z_block,
    t_shape
} Shape;

typedef enum {
    BEGINNING = 1,
    END = 10
}Gravity;

typedef struct {            // board width is 16 but im only using 10 ig
    uint16_t* arr;
    int height;
    int width;
}Board;

typedef struct {
    int height_top;            // where the top part of the tetrimino is
    int left_side;
    int right_side; 
    uint16_t bitmask[4];   // basic idea is i will have the 4 different state of the tetrimino next to eacah other consequentually
    Tetrimino_State tetrimino_state;
} Tetrimino;

typedef struct {
    Gravity grav;
    Gamestate gs;
    Board* locked_board;
    Board* moving_board;
    Tetrimino* moving_tetrimino;
} Tetris; 

static const uint16_t all_shape_Tetrimino_State_Map[t_shape + 1][4] = { // key is the tetrimino_state(int), the value is the bitmap, later from rotation we can decide which part of the tetrimino bitmap we can use
    {       // left_l_shape
        4,                    // first _row of bitmask
        30244,                // second _row of bitmask
        17126,                // ...
        512               
    },
    {       // right_l_shape
        2,
        18146,
        29734,
        1024
    },      
    {       // square
        26214,
        26214,
        0,
        0
    },      
    {       // I_block
        514,
        62194,
        514,
        514
    },
    {       // left_z_block
        16448,
        25443,
        9766,
        0
    }, 
    {       // right_z_block,
        8224,
        27756,
        17990,
        0
    },
    {       // t_shape
        0,
        18210,
        25191,
        16416
    }
};


void print_board(Tetris*);
Tetris* init_game();
Board* init_board(int);
void game_loop(Tetris*);
Tetrimino* init_tetrimino();
bool collision(const Tetris*, int);

int main () {

    Tetris* tetris = init_game();

    game_loop(tetris);


    return 0;
}

Tetris* init_game(){
    Gravity grav = BEGINNING;
    int height = HEIGHT + 2; // plus 2 is for the hidden layer
    int width  = 16;  
    Gamestate gs = INGAME;

    Tetris* tetris = malloc(sizeof(Tetris));
    tetris -> grav = grav;
    tetris -> gs = gs;
    tetris -> locked_board = init_board(height);
    tetris -> moving_board = init_board(height);
    tetris -> moving_tetrimino = init_tetrimino();

    return tetris;
}

Board* init_board(int height) {
    Board* board = malloc(sizeof(Board));
    board -> arr = malloc(sizeof(uint16_t) * height);
    board -> height = height; 
    board -> width  = 16;

    for (int i = 0; i < height; i++) {
        board -> arr[i] = 0;
    } 

    return board;
}

Tetrimino* init_tetrimino() {
    Tetrimino* tetrimino = calloc(1, sizeof(Tetrimino));

    tetrimino -> bitmask[0] |= (3 << 1);
    tetrimino -> bitmask[1] |= (3 << 1);
    tetrimino -> bitmask[2] = 0;
    tetrimino -> bitmask[3] = 0;

    tetrimino -> height_top = 0;
    tetrimino -> left_side = 6;
    tetrimino -> right_side = 5; 

    tetrimino -> tetrimino_state = Neutral;

    return tetrimino;
}

void game_loop(Tetris* tetris) {
    while (tetris -> gs == INGAME)
    {
        int i = 0;
        for (; !collision(tetris, tetris-> moving_tetrimino->height_top); i++){
            tetris -> moving_board = tetris -> locked_board;
            tetris -> moving_board -> arr[i] = tetris -> moving_tetrimino -> bitmask[i];
            tetris -> moving_board -> arr[i+1] = tetris -> moving_tetrimino -> bitmask[i+1];
        }
        tetris -> locked_board -> arr[i] += tetris -> moving_tetrimino -> bitmask[i];
        tetris -> locked_board -> arr[i + 1] += tetris -> moving_tetrimino -> bitmask[i + 1];
        print_board(tetris);
        break;
    }
}

void print_board(Tetris* tetris) {
    int h = tetris -> locked_board -> height;
    int w = tetris -> locked_board -> width;
    uint16_t* board_arr = tetris -> locked_board -> arr;

    for (int i = 0; i < h; i++) {
        int value_of_the_board_space = *(board_arr + i);

        for (int j = w - 1; j > 0; j--){
            if ( (value_of_the_board_space & (1 << j)) != 0 ) {
                printf("X");
            }
            else {
                printf("_");
            }
        }
        printf("%d\n", value_of_the_board_space);
    }

    return;
}

bool collision(const Tetris* tetris, int next_top_row) {
    int h = tetris -> locked_board -> height;
    int w = tetris -> locked_board -> width;
    Tetrimino* moving_tetrimino = tetris -> moving_tetrimino;
    const Board* locked = tetris -> locked_board;

    for (int r = 0; r < 4; r++) {
        uint16_t piece_row = moving_tetrimino -> bitmask[r];
        if (piece_row == 0) {
            continue;
        }

        int board_row = next_top_row + r;

        if (board_row >= locked -> height) {
            return true;
        }

        if ((locked -> arr[board_row] & piece_row) != 0) {
            return true;
        }
    } 

    return false;
}

void lock_piece (Tetris* tetris) {
    Board* locked = tetris -> locked_board;
    Tetrimino* moving_tetrimino = tetris -> moving_tetrimino;

    for (int r = 0; r < 4; r++) {
        uint16_t piece_row = moving_tetrimino -> bitmask[r];

        if (piece_row == 0) continue;

        int board_row = moving_tetrimino -> height_top + r;

        if (board_row >= 0 && board_row < locked -> height) 
            locked -> arr[board_row] | piece_row;
    }
}