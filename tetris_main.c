#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>

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
    BEGINNING = 1,
    END = 10
}Gravity;

typedef struct {
    uint16_t* arr;
}Board;

typedef struct {
    Gravity grav;
    Gamestate gs;
    Board* locked_board;
    Tetrimino* moving_tetrimino;
} Tetris; 

typedef struct {
    int area;
    uint16_t bitmask[4];   // basic idea is i will have the 4 different state of the tetrimino next to eacah other consequentually
} Tetrimino;

Tetris* init_game();
Board* init_board(int);
void game_loop(Tetris*);
Tetrimino* init_tetrimino();

int main () {

    Tetris* tetris = init_game();

    return 0;
}

Tetris* init_game(){
    Gravity grav = BEGINNING;
    int height = HEIGHT + 2; // plus 2 is for the hidden layer
    Gamestate gs = INGAME;

    Tetris* tetris = malloc(sizeof(Tetris));
    tetris -> grav = grav;
    tetris -> gs = gs;
    tetris -> locked_board = init_board(height);
    tetris -> moving_tetrimino = init_tetrimino();

    return tetris;
}

Board* init_board(int height) {
    Board* board = malloc(sizeof(Board));
    board -> arr = malloc(sizeof(uint16_t) * height);

    return board;
}

Tetrimino* init_tetrimino() {
    Tetrimino* tetrimino = malloc(sizeof(Tetrimino));


    return tetrimino;
}

void game_loop(Tetris* tetris) {
    while (tetris -> gs == INGAME)
    {
        
    }
}