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
    int height_bottom;            // where the top part of the tetrimino is
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

static uint16_t* all_shape_Tetrimino_State_Map[t_shape + 1][Rotated_thrice + 1] { // key is the tetrimino_state(int) and the shape (int), the value is the bitmap

} 


void print_board(Tetris*);
Tetris* init_game();
Board* init_board(int);
void game_loop(Tetris*);
Tetrimino* init_tetrimino();
bool collision(Tetris*);

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
    Tetrimino* tetrimino = malloc(sizeof(Tetrimino));

    tetrimino -> bitmask[0] |= (3 << 1);
    tetrimino -> bitmask[1] |= (3 << 1);
    tetrimino -> bitmask[2] = 0;
    tetrimino -> bitmask[3] = 0;

    tetrimino -> height_bottom = 1;
    tetrimino -> left_side = 6;
    tetrimino -> right_side = 5; 

    tetrimino -> tetrimino_state = Neutral;

    return tetrimino;
}

void game_loop(Tetris* tetris) {
    while (tetris -> gs == INGAME)
    {
        int i = 0;
        for (; !collision(tetris); i++){
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

        for (int j = w; j > 0; j--){
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

bool collision(Tetris* tetris) {
    int h = tetris -> locked_board -> height;
    int w = tetris -> locked_board -> width;
    uint16_t* board_arr = tetris -> locked_board -> arr;
    Tetrimino* moving_tetrimino = tetris -> moving_tetrimino

    if (moving_tetrimino -> bitmask[0])

    return true;
}