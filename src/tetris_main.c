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
#define MAX_FRAME_TIME 0.1

#define INPUT_BUFFER_SPEED 12

%:define something <% char name <:4:> = "Bob"; %>;

#define SHAPE_QUEUE_LEN 5

#define ACTUAL_ROTATION_RIGHT tetris -> moving_tetrimino -> tetrimino_state = (tetris -> moving_tetrimino -> tetrimino_state + 1) % 4;                
#define ROTATION_RIGHT (tetris -> moving_tetrimino -> tetrimino_state + 1) % 4  

#define ACTUAL_ROTATION_LEFT tetris -> moving_tetrimino -> tetrimino_state = (tetris -> moving_tetrimino -> tetrimino_state + 3) % 4;
#define ROTATION_LEFT (tetris -> moving_tetrimino -> tetrimino_state + 3) % 4

#define SCORE_NEED_FOR_GRAV_RAMPING_UP 100

#define VISIBLE_HEIGHT 20
#define VISIBLE_WIDTH 10

#define HIDDEN_LAYER_SIZE 2
#define HIDDEN_WIDTH_SIZE 6

#define LOGICAL_WIDTH 480
#define LOGICAL_HEIGHT 640

#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH 640

#define BLOCK_SIZE 28

#define BOARD_X ( LOGICAL_WIDTH - BOARD_W ) / 2
#define BOARD_Y ( LOGICAL_HEIGHT - BOARD_H ) / 2

#define BOARD_W (VISIBLE_WIDTH * BLOCK_SIZE) 
#define BOARD_H (VISIBLE_HEIGHT * BLOCK_SIZE)

#define DEFAULT_COLOR 255,255,255

#define TETRIMINO_BOX_HEIGHT 4

// macros in the future #define TYPE_OF_INT uint_ ## (HIDDEN_WIDTH + VISIBLE_WIDTH)

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
                            // we need the 0 because how we implemented grav_ramping later on
Gravity grav_ramp_states[] = {0,RAMP0,RAMP1,RAMP2,RAMP3,RAMP4,RAMP5,RAMP6,RAMP7,RAMP8,RAMP9,END};

                            //single, double, triple, tetris
uint8_t simple_score_board[] = {0,1,3,5,8};

typedef enum {
    MOV_LEFT,
    MOV_RIGHT,
    MOV_DOWN_SLOW,
    DROP_DOWN,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    TRAP,
    QUIT
} Input_types;

typedef struct {
    Input_types input_type;
    bool is_pressed;
    bool is_released;
} Input;

struct Board{            // board width is 16 but im only using 10 ig
    uint16_t* arr;
    int height;
    int width;
} ;

typedef struct Tetrimino_Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Tetrimino_Color;
                                                                                                                                                            // locked board square color                        
Tetrimino_Color color_map[t_shape + 2] = { {253u, 63u, 89u}, {255u, 200u, 46u}, {254u, 251u, 52u}, {83u, 218u, 63u}, {1u, 237u, 250u}, {221u, 10u, 178u}, {234u, 20u, 28u}, {225u, 248u, 220u}};

struct Tetrimino{
    int height_top;            // where the top part of the tetrimino is
    int x_right; // the right side of the tetrimino x
    Tetrimino_State tetrimino_state;
    Shape shape;
} ;

struct Tetris{
    Gravity grav;
    Gamestate gs;
    Board* locked_board;
    Tetrimino* moving_tetrimino;
    Queue* tetrimino_shape_queue;
    uint64_t score;
    double input_buffer_speed;
} ; 

struct Queue{
    Shape shapes[SHAPE_QUEUE_LEN];
    int front;
    int rear;
    int lenght;
    int size;
} ;

typedef struct Rotation_mask {
    uint8_t rows[4];
    uint8_t width;
    int height;
} Rotation_mask;

static const Rotation_mask all_shape_Tetrimino_State_Map[t_shape + 1][Rotated_thrice +1] = { 
    [left_l_shape] = {
        {
            .rows = {
                0b0001,
                0b0001,
                0b0011,
                0b0000
            },
            .width = 2,
            .height = 3
        }, 
        {
            .rows = {
                0b0100,
                0b0111,
                0b0000,
                0b0000
            },
            .width = 3,
            .height = 2
        }, 
        {
            .rows = {
                0b0011,
                0b0010,
                0b0010,
                0b0000
            },
            .width = 2,
            .height = 3
        }, {
            .rows = {
                0b0111,
                0b0001,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        }
    },
    [right_l_shape] = {
        {
            .rows = {
                0b0001,
                0b0001,
                0b0011,
                0b0000
            },
            .width = 2,
            .height = 3
        },
        {
            .rows = {
                0b0100,
                0b0111,
                0b0000,
                0b0000
            },
            .width = 3,
            .height = 2
        }, {
            .rows = {
                0b0011,
                0b0010,
                0b0010,
                0b0000
            },
            .width = 2,
            .height = 3
        }, {
            .rows = {
                0b0111,
                0b0001,
                0b0000,
                0b0000
            },
            .width = 3,
            .height = 2
        }
    },      
    [square] = {  
        {
            .rows = {
                0b0011,
                0b0011,
                0b0000,
            },
            .height = 2,
            .width = 2
        },
        {
            .rows = {
                0b0011,
                0b0011,
                0b0000,
            },
            .height = 2,
            .width = 2
        },
        {
            .rows = {
                0b0011,
                0b0011,
                0b0000,
            },
            .height = 2,
            .width = 2
        },
        {
            .rows = {
                0b0011,
                0b0011,
                0b0000,
            },
            .height = 2,
            .width = 2
        }        
    },      
    [I_block] = {       
        {
            .rows = {
                0b0001,
                0b0001,
                0b0001,
                0b0001
            },
            .height = 4,
            .width = 1
        },
        {
            .rows = {
                0b1111,
                0b0000,
                0b0000,
                0b0000
            },
            .height = 1,
            .width = 4
        },
        {
            .rows = {
                0b0001,
                0b0001,
                0b0001,
                0b0001
            },
            .height = 4,
            .width = 1
        },
        {
            .rows = {
                0b1111,
                0b0000,
                0b0000,
                0b0000
            },
            .height = 1,
            .width = 4
        }
    },
    [left_z_block] = {       
        {
            .rows = {
                0b0110,
                0b0011,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0001,
                0b0011,
                0b0010,
                0b0000
            },
            .height = 3,
            .width = 2
        },
        {
            .rows = {
                0b0110,
                0b0011,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0001,
                0b0011,
                0b0010,
                0b0000
            },
            .height = 3,
            .width = 2
        }
    },
    [right_z_block] = {       
        {
            .rows = {
                0b0011,
                0b0110,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0010,
                0b0011,
                0b0001,
                0b0000
            },
            .height = 3,
            .width = 2
        },
        {
            .rows = {
                0b0011,
                0b0110,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0010,
                0b0011,
                0b0001,
                0b0000
            },
            .height = 3,
            .width = 2
        }
    },
    [t_shape] = {       
        {
            .rows = {
                0b0010,
                0b0111,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0010,
                0b0011,
                0b0010,
                0b0000
            },
            .height = 3,
            .width = 2
        },
        {
            .rows = {
                0b0111,
                0b0010,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0001,
                0b0011,
                0b0001,
                0b0000
            },
            .height = 3,
            .width = 2
        }
    }
};

Input_types input_mapper(const bool*);
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
double get_time(void);
void update(Tetris*);
bool SDL_init();
void render_current_frame(SDL_Renderer*,  Tetris*);
void set_tetrimino_color(SDL_Renderer*, Shape);
void input_handler(Input_types input_conv, Tetris* tetris);

/*
new render logic: 

when board changes:
    scan locked board array once
    draw locked cells into texture

every frame:
    clear screen
    copy locked-board texture
    draw moving piece
    present
*/

// implement adding to score at eacch line clear
// better sdl3 visuals
// edging


static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

int main (void) {
    Tetris* tetris = init_game();

    if (!SDL_init()) {
        return 1;
    }

    game_loop(tetris);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free_tetris(tetris);

    return 0;
}

bool SDL_init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("Tetris_in_C", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("couldnt create window and renderer: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    SDL_SetRenderLogicalPresentation(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    return true;
}


void game_loop(Tetris *tetris)
{
    double previous_time = get_time();
    double delta_time = 0.0;

    uint8_t current_grav = 1;
    
    tetris -> grav = grav_ramp_states[current_grav];

    Input_types last_input = TRAP;

    while (tetris->gs == INGAME) {

        double now = get_time();

        double frame_time = now - previous_time;
        previous_time = now;

        // just to make sure we dont update like crazy after one lag
        if (frame_time > MAX_FRAME_TIME) {
            frame_time = MAX_FRAME_TIME;
        }

        delta_time += frame_time;

        if (tetris -> grav != END ) {
            if (tetris -> score / SCORE_NEED_FOR_GRAV_RAMPING_UP >= current_grav) {
                current_grav += 1;
                tetris -> grav = grav_ramp_states[current_grav];
            }
        }

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                tetris->gs = LOST;
            }
        }

        const bool *keys = SDL_GetKeyboardState(NULL);
    
        Input_types current_input = input_mapper(keys);
        if (current_input != last_input) {
            input_handler(input_mapper(keys), tetris);
        }
        last_input = current_input;

        double drop_interval = (double)tetris -> grav / 100.0;
        while (delta_time >= drop_interval) {
            update(tetris);
            delta_time -= drop_interval;
        }
        
        
        SDL_SetRenderDrawColor(renderer,DEFAULT_COLOR,SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        render_current_frame(renderer, tetris); 

        SDL_RenderPresent(renderer);

        double frame_end = get_time();
        double frame_duration = frame_end - now;
        double remain_time = TARGET_FRAME_TIME - frame_duration;

        if (remain_time > 0.0) {
            SDL_Delay((Uint32)(remain_time * 1000.0));
        }
    }
    if (tetris -> gs == LOST) { return; } 
    if (tetris -> gs == WON) { return; } 
    exit(1); // you either lose or win but somehow got something else just quit
}

// current frame rendering is 480, 640 -> will stretch --> 10 block wide 
// each block should be 28x28
void set_tetrimino_color(SDL_Renderer *renderer, Shape shape) {
    SDL_SetRenderDrawColor(renderer, color_map[shape].r,color_map[shape].g, color_map[shape].b, SDL_ALPHA_OPAQUE);
    return;
}

void input_handler(Input_types input_conv, Tetris* tetris) {
    switch (input_conv) {
        case MOV_LEFT:{
            
            int next_right_x = tetris->moving_tetrimino->x_right - 1;
            if (side_collision(tetris, next_right_x)) {
                break;
            }
            tetris->moving_tetrimino->x_right = next_right_x;
            break;
        }

        case MOV_RIGHT:{ 
            int next_right_x = tetris->moving_tetrimino->x_right + 1;
            if (side_collision(tetris, next_right_x)) {
                break;
            }
            tetris->moving_tetrimino->x_right = next_right_x;
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

    Tetrimino* tetrimino = tetris->moving_tetrimino;
    set_tetrimino_color(renderer, tetrimino->shape);

    for (int piece_row = 0; piece_row < TETRIMINO_BOX_HEIGHT; piece_row++) {
        uint16_t value_row = get_current_piece_row(tetrimino, piece_row);

        if (value_row == 0) {
            continue;
        }

        int board_row = tetrimino->height_top + piece_row;
        int screen_row = board_row;

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
}

void update(Tetris* tetris) {
    int next_top = tetris->moving_tetrimino->height_top+1;
    if (fall_collision(tetris, next_top)) {
        place_clear_over_new(tetris);
    } else {
        tetris->moving_tetrimino->height_top = next_top; 
    }
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

    return TRAP;
}

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
    board -> width  = VISIBLE_WIDTH + HIDDEN_WIDTH_SIZE;

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
    tetrimino -> height_top = 0;
    tetrimino -> x_right = 3;
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

void init_queue(Queue* q) {
    q -> front = 0;
    q -> rear = 0;
    q -> lenght = SHAPE_QUEUE_LEN;
    q -> size = 0;
}

void free_queue(Queue* q) { 
    free(q);
}

bool is_empty(Queue* q) { 
    return q->size == 0;
}

bool is_full(Queue* q) {
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
    return;
}

Shape dequeue(Queue* q) {
    // remove first element
    if (is_empty(q)) {
        printf("Queue is empty\n");
        return -1;
    }
    
    Shape shape = q->shapes[q->front];
    q->front = (++q->front) % q -> lenght;
    q->size--;
    return shape;
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
            row += 1;
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

double get_time(void) {
    return (double)SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();
}