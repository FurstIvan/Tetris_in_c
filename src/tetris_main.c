#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define TARGET_FPS 60.0
#define FIXED_STEP (1.0 / 60.0)
#define MAX_FRAME_TIME 0.25   

%:define something <% char name <:3:> = "Bob"; %>;

#define HEIGHT 20
#define WIDTH 10
#define SHAPE_QUEUE_LEN 5

#define ROTATION_RIGHT do {                                                                                                    \
        tetris -> moving_tetrimino -> tetrimino_state = (tetris -> moving_tetrimino -> tetrimino_state + 1) % 4;               \
    } while (0);  
    
#define ROTATION_LEFT do {                                                                                                    \
        tetris -> moving_tetrimino -> tetrimino_state = (tetris -> moving_tetrimino -> tetrimino_state + 3) % 4;              \
    } while (0);  

void rotate_right(Tetrimino* t);
void rotate_left(Tetrimino* t);

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

typedef enum {
    MOV_LEFT,
    MOV_RIGHT,
    MOV_DOWN_SLOW,
    DROP_DOWN,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    TRAP,
    QUIT
} Inputs;

typedef struct {            // board width is 16 but im only using 10 ig
    uint16_t* arr;
    int height;
    int width;
}Board;

typedef struct {
    int height_top;            // where the top part of the tetrimino is
    int left_x;
    uint16_t bitmask[4];   // basic idea is i will have the 4 different state of the tetrimino next to eacah other consequentually
    Tetrimino_State tetrimino_state;
    Shape shape;
} Tetrimino;

typedef struct {
    Gravity grav;
    Gamestate gs;
    Board* locked_board;
    Tetrimino* moving_tetrimino;
    Queue* tetrimino_shape_queue;
} Tetris; 

static const uint16_t all_shape_Tetrimino_State_Map[t_shape + 1][4] = { 
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

Inputs input_mapper(const bool*);
void print_board(Tetris*);
Tetris* init_game();
Board* init_board(int);
void game_loop(Tetris*);
Tetrimino* init_tetrimino(Shape shape);
bool collision(const Tetris*, int);
void hard_drop(Tetris*);

typedef struct
{
    Shape shapes[SHAPE_QUEUE_LEN];
    int front;
    int rear;
} Queue;

void init_queue(Queue *q) {
    // init que

    q->front = -1;
    q->rear = 0;
}

bool is_empty(Queue *q) // chekc for empty
{
    return (q->front == q->rear - 1);
}

bool is_full(Queue *q) {
    // check fullness
    return (q->rear == SHAPE_QUEUE_LEN);
}

void enqueue(Queue *q, Shape shape) {
    // add an element to the back
    if (is_full(q))
    {
        printf("Queue is full\n");
        return;
    }
    q->shapes[q->rear] = shape;
    q->rear++;
}

Shape dequeue(Queue *q) {
    // remove first element
    if (is_empty(q))
    {
        printf("Queue is empty\n");
        return -1;
    }
    else {
        return q->shapes[q->front + 1];
    }
    q->front++;
}

void print_queue(Queue *q) {
    if (is_empty(q))
    {
        printf("Queue is empty\n");
        return;
    }

    printf("Current Queue: ");
    for (int i = q->front + 1; i < q->rear; i++)
    {
        printf("%d ", q->shapes[i]);
    }
    printf("\n");
}

int main () {
    Tetris* tetris = init_game();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_INIT_FAIL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Tetris_in_C", 600, 800, SDL_WINDOW_FULLSCREEN);
    
    if (!window) {
        SDL_Log("SDL_CREATE_WINDOW FAIL %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    game_loop(tetris);
    return 0;
}

void game_loop(Tetris *tetris)
{
    while (tetris->gs == INGAME) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                tetris->gs = LOST;
            }
        }

        const bool *keys = SDL_GetKeyboardState(NULL);
        Inputs input_conv = input_mapper(keys);

        switch (input_conv) {
            case MOV_LEFT:
                tetris->moving_tetrimino->left_x++;

                if (collision(tetris, tetris->moving_tetrimino->height_top)) {
                    tetris->moving_tetrimino->left_x--;
                }

                break;

            case MOV_RIGHT:
                tetris->moving_tetrimino->left_x--;

                if (collision(tetris, tetris->moving_tetrimino->height_top)) {
                    tetris->moving_tetrimino->left_x++;
                }

                break;

            case MOV_DOWN_SLOW:
                tetris->moving_tetrimino->height_top--;

                if (collision(tetris, tetris->moving_tetrimino->height_top)) {
                    tetris->moving_tetrimino->height_top++;
                }

                break;

            case DROP_DOWN:
                hard_drop(tetris);
                break;

            case ROTATE_LEFT:
                ROTATION_LEFT;

                if (collision(tetris, tetris->moving_tetrimino->height_top)) {
                    ROTATION_RIGHT;
                }

                break;

            case ROTATE_RIGHT:
                ROTATION_RIGHT;

                if (collision(tetris, tetris->moving_tetrimino->height_top)) {
                    ROTATION_LEFT;
                }

                break;

            case QUIT:
                tetris->gs = LOST;
                break;

            case TRAP:
                break;

            default:
                break;
        }

        print_board(tetris);
    }
    if (tetris -> gs == LOST) { return; } 
    if (tetris -> gs == WON) { return; } 
    exit(1); // you either lose or win of somehow got something else just quit
}


Inputs input_mapper(const bool *keys) {
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

    if (keys[SDL_SCANCODE_Q]) {
        return ROTATE_RIGHT;
    }

    return TRAP;
}

Tetris* init_game(){
    Gravity grav = BEGINNING;
    int height = HEIGHT + 2; // plus 2 is for the hidden layer
    int width  = 16;  
    Gamestate gs = INGAME;

    Tetris* tetris = malloc(sizeof(Tetris));
    tetris -> tetrimino_shape_queue = malloc(sizeof(Queue));
    init_queue(tetris -> tetrimino_shape_queue);

    tetris -> grav = grav;
    tetris -> gs = gs;
    tetris -> locked_board = init_board(height);

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

Board* init_board(int height) {
    Board* board = calloc(1, sizeof(Board));
    board -> arr = calloc(1, sizeof(uint16_t) * height);
    board -> height = height; 
    board -> width  = 16;

    for (int i = 0; i < height; i++) {
        board -> arr[i] = 0;
    } 

    return board;
}

Tetrimino* init_tetrimino(const Shape shape) {
    Tetrimino* tetrimino = calloc(1, sizeof(Tetrimino));
    tetrimino -> shape = shape;

    for (int i = 0; i < 4; i++) {
        tetrimino -> bitmask[i] |= all_shape_Tetrimino_State_Map[shape][i];
    }

    tetrimino -> height_top = 0;
    tetrimino -> left_x = 6; 

    tetrimino -> tetrimino_state = Neutral;

    return tetrimino;
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

    if (moving_tetrimino -> left_x == 10 || moving_tetrimino -> left_x - 4 == 0)
        return true;

    for (int r = 0; r < 4; r++) {
        uint16_t piece_row = moving_tetrimino -> bitmask[r];

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
            locked -> arr[board_row] |= piece_row;
    }
}

void rotate_right(Tetrimino* t) {
    t -> tetrimino_state = (t -> tetrimino_state + 1) % 4;
}
void rotate_left(Tetrimino* t) {
    t -> tetrimino_state = (t -> tetrimino_state + 3) % 4;
}

void place_tetrimino(Tetris* tetris, int height_top, int left) {
    Tetrimino* tetrimino = tetris -> moving_tetrimino;
    Tetrimino_State state = tetrimino -> tetrimino_state;
    Board* board = tetris ->locked_board;

    for (int i = 0; i < 4; i++) {
        uint16_t tetrimino_row = tetrimino -> bitmask[i];
        int amount_of_bits_per_window = 4;
        int starting_bit = state * 4;

        uint16_t mask = (1u << amount_of_bits_per_window) - 1u; // this is giving us an array full of 1-s that we can later use to make a result
        uint16_t result =((mask << starting_bit) & tetrimino_row) >> starting_bit; // this will only be true where there is a 1 or at the specific place we need, and then we normalize it

        uint16_t row_ready = result << left;

        board -> arr[height_top + i] |= row_ready; 
    }
    return;
}

void get_new_tetrimino(Tetris* tetris) {
    Tetrimino* tetrimino = tetris -> moving_tetrimino;
    tetrimino -> shape = dequeue(tetris -> tetrimino_shape_queue);

    for (int i = 0; i < 4; i++) {
        tetrimino -> bitmask[i] |= all_shape_Tetrimino_State_Map[tetrimino -> shape][i];
    }

    tetrimino -> height_top = 0;
    tetrimino -> left_x = 6; 

    tetrimino -> tetrimino_state = Neutral;

    Shape random_shape = rand() % (t_shape + 1);
    enqueue(tetris -> tetrimino_shape_queue, random_shape);

    return;
}

void free_tetrimino(Tetrimino* tetrimino) {
    free(tetrimino);
}

void hard_drop(Tetris* tetris) {
    // the algo i came up with, check from tetrimino level downwards, untill collsion 
    Tetrimino* tetrimino = tetris -> moving_tetrimino;
    Board* board = tetris -> locked_board;
    int height = board -> height;
    int bottom_part_tetrimino = tetrimino -> height_top + 3;

    for (;bottom_part_tetrimino != height; bottom_part_tetrimino++) {
        if (collision(tetris, bottom_part_tetrimino)) {
            place_tetrimino(tetris, bottom_part_tetrimino - 1, tetrimino -> left_x);
            get_new_tetrimino(tetris);
        }
    }

    return;
}

