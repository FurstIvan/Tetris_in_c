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
#define TARGET_FRAME_TIME (1.0 / TARGET_FPS)
#define MAX_FRAME_TIME 0.25   

%:define something <% char name <:4:> = "Bob"; %>;

#define VISIBLE_HEIGHT 20
#define VISIBLE_WIDTH 10
#define SHAPE_QUEUE_LEN 5

#define ACTUAL_ROTATION_RIGHT tetris -> moving_tetrimino -> tetrimino_state = (tetris -> moving_tetrimino -> tetrimino_state + 1) % 4;                
#define ROTATION_RIGHT (tetris -> moving_tetrimino -> tetrimino_state + 1) % 4  

#define ACTUAL_ROTATION_LEFT tetris -> moving_tetrimino -> tetrimino_state = (tetris -> moving_tetrimino -> tetrimino_state + 3) % 4;
#define ROTATION_LEFT (tetris -> moving_tetrimino -> tetrimino_state + 3) % 4

// think as in an array of row masks

typedef struct Tetrimino Tetrimino;
typedef struct Queue Queue;
typedef struct Tetris Tetris;
typedef struct Board Board;

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
    RAMP0 = 100,
    RAMP1 = 95,
    RAMP2 = 85,
    RAMP3 = 75,
    RAMP4 = 55,
    RAMP5 = 45,
    RAMP6 = 40,
    RAMP7 = 30,
    RAMP8 = 20,
    RAMP9 = 15,
    END   = 10
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

struct Board{            // board width is 16 but im only using 10 ig
    uint16_t* arr;
    int height;
    int width;
} ;

struct Tetrimino{
    int height_top;            // where the top part of the tetrimino is
    int left_x;
    uint16_t bitmask[4];   // basic idea is i will have the 4 different state of the tetrimino next to eacah other consequentually
    Tetrimino_State tetrimino_state;
    Shape shape;
} ;

struct Tetris{
    Gravity grav;
    Gamestate gs;
    Board* locked_board;
    Tetrimino* moving_tetrimino;
    Queue* tetrimino_shape_queue;
} ; 

struct Queue{
    Shape shapes[SHAPE_QUEUE_LEN];
    int front;
    int rear;
    int lenght;
    int size;
} ;

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
// game loop and fps figure out

// sdl3 getting it to work... 

Inputs input_mapper(const bool*);
void print_board(Tetris*);
Tetris* init_game();
Board* init_board(int);
void game_loop(Tetris*);
Tetrimino* init_tetrimino(Shape);
void hard_drop(Tetris*);
void rotate_right(Tetrimino*);
void rotate_left(Tetrimino*);
uint16_t get_current_piece_row(Tetrimino*, int);
void free_tetrimino(Tetrimino*);
void get_new_tetrimino(Tetris*);
void place_tetrimino(Tetris*, int, int);
void init_queue(Queue*);
bool is_empty(Queue*);
bool is_full(Queue*);
void enqueue(Queue*, Shape);
Shape dequeue(Queue*);
void print_queue(Queue*);
void line_clear_check(Tetris*); 
void line_clear(Board*, int);
void game_over_check(Tetris* tetris);
int main(void);
bool fall_collision(Tetris*, int);
bool side_collision(Tetris*, int);
bool rotation_collision(Tetris*, Tetrimino_State);
void place_clear_over_new(Tetris*);
void free_tetris(Tetris*);
void free_board(Board*);
void free_queue(Queue*);
uint64_t get_time(void);

// add a way to ramp up the gravity each time you get eough scores --> implement score system too
// learn about x_macros

int main (void) {
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

    SDL_DestroyWindow(window);
    SDL_Quit();

    free_tetris(tetris);

    return 0;
}

void game_loop(Tetris *tetris)
{
    uint64_t game_time = get_time();
    uint64_t previous_time = get_time();
    uint64_t gravity_timer = 0.0;

    if (game_time >= 120.0) {
        tetris -> grav += 1;
    }
    Gravity GRAVITY_INTERVAL = tetris -> grav;

    while (tetris->gs == INGAME) {

        uint64_t frame_start = get_time();

        uint64_t change_time = frame_start - previous_time;
        previous_time = frame_start;

        if (change_time > MAX_FRAME_TIME) {
            change_time = MAX_FRAME_TIME;
        }

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                tetris->gs = LOST;
            }
        }

        const bool *keys = SDL_GetKeyboardState(NULL);
        Inputs input_conv = input_mapper(keys);

        switch (input_conv) {
            case MOV_LEFT:{
                int next_left_x = tetris->moving_tetrimino->left_x + 1;
                if (side_collision(tetris, next_left_x)) {
                    break;
                }
                tetris->moving_tetrimino->left_x = next_left_x;
                break;
            }

            case MOV_RIGHT:{ 
                int next_left_x = tetris->moving_tetrimino->left_x - 1;
                if (side_collision(tetris, next_left_x)) {
                    break;
                }
                tetris->moving_tetrimino->left_x = next_left_x;
                break;
            }

            case MOV_DOWN_SLOW:{
                int next_height = tetris->moving_tetrimino->height_top+1;
                if (fall_collision(tetris,next_height)) {
                    place_clear_over_new(tetris);
                    break;
                }
                tetris->moving_tetrimino->height_top = next_height;
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

            case TRAP:{
                break;
            }

            default: {
                break;
            }
        }
       
        gravity_timer += change_time;
    
        if (gravity_timer >= GRAVITY_INTERVAL) {
            fall_one_step(tetris);
            gravity_timer = 0.0;
        }


        print_board(tetris);

        uint64_t frame_end = get_time();
        uint64_t frame_duration = frame_end - frame_start;
        uint64_t remain_time = TARGET_FRAME_TIME - frame_duration;

        if (remain_time > 0.0) {
            SDL_Delay((uint32_t)(remain_time * 1000.0));
        }
    }
    if (tetris -> gs == LOST) { return; } 
    if (tetris -> gs == WON) { return; } 
    exit(1); // you either lose or win of somehow got something else just quit
}

void fall_one_step(Tetris* tetris) {
    int next_top = tetris->moving_tetrimino->height_top+1;
    if (fall_collision(tetris, next_top)) {
        place_clear_over_new(tetris);
    } else {
        tetris->moving_tetrimino->height_top = next_top; 
    }
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
    Gravity grav = RAMP0;
    int height = VISIBLE_HEIGHT + 2; // plus 2 is for the hidden layer
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

void free_board(Board* board) {
    free(board -> arr);
    free(board);
    return;
}

Tetrimino* init_tetrimino(const Shape shape) {
    Tetrimino* tetrimino = calloc(1, sizeof(Tetrimino));
    tetrimino -> shape = shape;

    for (int i = 0; i < 4; i++) {
        tetrimino -> bitmask[i] |= all_shape_Tetrimino_State_Map[shape][i];
    }

    tetrimino -> height_top = 0;
    tetrimino -> left_x = 3; 

    tetrimino -> tetrimino_state = Neutral;

    return tetrimino;
}

void free_tetrimino(Tetrimino* tetrimino) {
    free(tetrimino);
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

bool fall_collision(Tetris* tetris, int next_top_row) {
    int h = tetris -> locked_board -> height;

    Tetrimino* moving_tetrimino = tetris -> moving_tetrimino;
    const Board* locked = tetris -> locked_board;

    for (int r = 0; r < 4; r++) {
        uint16_t row_ready = get_current_piece_row(moving_tetrimino, r);
        int board_row = next_top_row + r;

        if (row_ready == 0) {continue;}

        if (board_row >= h && row_ready != 0) {
            return true;
        }

        if ((locked -> arr[board_row] & row_ready) != 0) {
            return true;
        }
    } 

    return false;
}

bool side_collision(Tetris* tetris, int next_left_x) {
    Tetrimino moving_tetrimino_copy = *(tetris -> moving_tetrimino);
    moving_tetrimino_copy.left_x = next_left_x;

    const Board* locked = tetris -> locked_board;

    for (int r = 0; r < 4; r++) {
        uint16_t after_moving = get_current_piece_row(&moving_tetrimino_copy, r);
        uint16_t current = get_current_piece_row(tetris -> moving_tetrimino, r);

        int board_row = moving_tetrimino_copy.height_top + r;

        if (after_moving == 0) {
            continue;
        }

        if (current % 2 == 1 && next_left_x < tetris -> moving_tetrimino -> left_x) {
            return true;
        } else if ( after_moving > ((1u << VISIBLE_WIDTH) - 1u) && next_left_x > tetris -> moving_tetrimino -> left_x)
        {
            return true;
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

    const Board* locked = tetris -> locked_board;

    for (int r = 0; r < 4; r++) {
        uint16_t after_moving = get_current_piece_row(&moving_tetrimino_copy, r);
        int board_row = moving_tetrimino_copy.height_top + r;

        if (after_moving == 0) {
            continue;
        }

        if (board_row < 0 || board_row >= VISIBLE_HEIGHT) {
            return true;
        }

        if (after_moving > ((1u << VISIBLE_WIDTH) - 1u)) {
            return true;
        }

        if ((locked->arr[board_row] & after_moving) != 0) {
            return true;
        }
    }

    return false;
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

    for (int i = 0; i < 4; i++) {
        tetrimino -> bitmask[i] = 0;
        tetrimino -> bitmask[i] |= all_shape_Tetrimino_State_Map[tetrimino -> shape][i];
    }

    tetrimino -> height_top = 0;
    tetrimino -> left_x = 3; 

    tetrimino -> tetrimino_state = Neutral;

    Shape random_shape = rand() % (t_shape + 1);
    enqueue(tetris -> tetrimino_shape_queue, random_shape);

    return;
}

void place_clear_over_new(Tetris* tetris) {
    int new_height_top = tetris->moving_tetrimino->height_top-1;
    place_tetrimino(tetris, new_height_top, tetris->moving_tetrimino -> left_x);
    line_clear_check(tetris);
    game_over_check(tetris);
    get_new_tetrimino(tetris);
}

void hard_drop(Tetris* tetris) {
    // the algo i came up with, check from tetrimino level downwards, untill collsion 
    Tetrimino* tetrimino = tetris -> moving_tetrimino;
    Board* board = tetris -> locked_board;
    int height = board -> height;
    int height_top = tetrimino -> height_top;

    for (;height_top != height - 3; height_top++) {
        tetrimino->height_top++;
        if (fall_collision(tetris, height_top + 1)) {
            place_clear_over_new(tetris);
            return;
        }
    }

    place_clear_over_new(tetris);
    return;
}

uint16_t get_current_piece_row(Tetrimino* tetrimino, int row) {
    uint16_t tetrimino_row = tetrimino -> bitmask[row];

    int amount_of_bits_per_window = 4;
    int starting_bit = tetrimino -> tetrimino_state * 4;

    uint16_t mask = (1u << amount_of_bits_per_window) - 1u; // this is giving us an array full of 1-s that we can later use to make a result
    uint16_t result =((mask << starting_bit) & tetrimino_row) >> starting_bit; // this will only be true where there is a 1 or at the specific place we need, and then we normalize it

    uint16_t row_ready = result << tetrimino -> left_x;

    return row_ready;
}


void init_queue(Queue* q) {
    // init queue

    q -> front = -1;
    q -> rear = 0;
    q -> lenght = SHAPE_QUEUE_LEN;
    q -> size = 0; 
}

void free_queue(Queue* q) { 
    free(q);
}

bool is_empty(Queue* q) // check for empty
{
    return q->size == 0;
}

bool is_full(Queue* q) {
    // check fullness
    return q->size == q->lenght;
}

void enqueue(Queue* q, Shape shape) {
    // add an element to the back
    if (is_full(q))
    {
        printf("Queue is full\n");
        return;
    }
    q->shapes[q->rear] = shape;
    q -> rear = (++q->rear) % q->lenght;
    q->size++;
}

Shape dequeue(Queue* q) {
    // remove first element
    if (is_empty(q))
    {
        printf("Queue is empty\n");
        return -1;
    }
    else {
        Shape shape = q->shapes[q->front];
        q->front = (++q->front) % q -> lenght;
        q->size--;
        return shape;
    }
}

void print_queue(Queue* q) {
    if (is_empty(q))
    {
        printf("Queue is empty\n");
        return;
    }

    printf("Current Queue: ");
    for (int i = 0; i < q->size; i++)
    {
        int j = (q->front + i) % q->lenght;
        printf("%d ", q->shapes[j]);
    }
    printf("\n");
}

void line_clear(Board* board, int row) {
    board -> arr[row] = 0;
    for (; row > 1; row--) {
        board -> arr[row] = board -> arr[row - 1];
    }
}

void line_clear_check(Tetris* tetris) {
    int line_clear_mask = (1u <<  VISIBLE_WIDTH) - 1;

    for (int row = tetris->locked_board->height - 1; row >= 2; row--) {
        if (tetris -> locked_board -> arr[row] == line_clear_mask) {
            line_clear(tetris -> locked_board, row);
            row + 1;
        }
    }
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

    free_queue(q);
    free_board(board);
    free_tetrimino(moving_tetrimino);
    free(tetris);
    return;
}

uint64_t get_time(void) {
    return SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
}