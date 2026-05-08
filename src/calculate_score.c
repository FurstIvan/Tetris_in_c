#include "calculate_score.h"

// something else to keep in mind is that we could run each board on a different thread if we can so that way we achieve multi threading within the game

// then after we get the desired board we render it ... plus we should be able to tell how far into the future to look


// one thread will have the task of doing one x_pos and all the rotation types fast_drop and then look at the next shape and there maybe create new threads and make some new 


// kinda like this:


/*
 
                *       first it has the task of row (1) for thread (1) -> hard drop it and the eval the position
                |
            ---------   (level 1)
            *       *   then if we desire so we can make more depth research with asking if the next shape comes down where to (kinda like x * 10 ) so we might not be able to go much lower
            |       |
           ---     ---  (level 2)
          *   *   *   *
          |   |   |   |
         ---    ...     (level 3)
        *   *
*/


#include "calculate_score.h"

// going to calculate the state of the board given a state of it with a tetrimino not set in a place !!! this is very important
// i will use drophard without actually setting the real piece just a copy of it
// also the piece should be set at a determined x coordinate, i will not s

static Board hard_copy_of_board(const Board*);
static bool fall_collision_sim(const Board*, const Tetrimino*, int16_t);
static bool rotation_collision_sim(const Board*, const Tetrimino*, Tetrimino_State);
static uint8_t clear_full_lines_sim(Board*);
static uint32_t score_board_after_line_clears(Board*);
static uint16_t count_holes(const Board*);
static uint8_t get_column_height(const Board*, uint8_t);
static uint16_t get_collective_height(const Board*);
static uint8_t get_max_height(const Board*);
static uint16_t get_bumpiness(const Board*);

Board* simulate_drop_hard_and_place(const Board* board, const Tetrimino* tetrimino) { // free it after use of board
    Board copy_board = hard_copy_of_board(board);
    Board* copy_board_ptr = malloc(sizeof(Board));

    *copy_board_ptr = copy_board;

    Tetrimino copy_tetrimino = *(tetrimino);

    uint8_t height_top_new = (uint8_t)tetrimino -> height_top;

    while (!fall_collision_sim(copy_board_ptr, &copy_tetrimino, height_top_new)) {
        height_top_new += 1;
    }

    copy_tetrimino.height_top = height_top_new - 1; // we subtract one because we find collision one below so we go up one

    for (int row = 0; row < TETRIMINO_BOX_HEIGHT; row++) {
        uint16_t after_falling = get_current_piece_row(&copy_tetrimino, row);

        if (after_falling == 0) {
            continue;
        }

        uint8_t board_row = copy_tetrimino.height_top + row;

        copy_board_ptr->arr[board_row] |= after_falling;
    }

    return copy_board_ptr;
}

static Board hard_copy_of_board(const Board* board) { // dont forget to free!
    Board board_copy = {0};

    board_copy.width = board -> width;
    board_copy.height = board -> height;

    board_copy.arr = calloc(1, sizeof(uint16_t) * board_copy.height);

    for (uint8_t i = 0; i < board_copy.height; i++) {
        board_copy.arr[i] = board -> arr[i];
    }

    return board_copy;
}

static bool fall_collision_sim(const Board* board, const Tetrimino* tetrimino, int16_t next_top_row) {

    Tetrimino copy_tetrimino = *(tetrimino);
    copy_tetrimino.height_top = next_top_row;

    if (next_top_row < 0) {
        return true;
    }

    const Rotation_mask* rotation = &(all_shape_Tetrimino_State_Map[copy_tetrimino.shape][copy_tetrimino.tetrimino_state]);

    if (board -> height - rotation -> height - copy_tetrimino.height_top < 0) {
        return true;
    }

    for (uint8_t row = 0; row < TETRIMINO_BOX_HEIGHT; row++) {
        uint16_t after_falling = get_current_piece_row(&copy_tetrimino, row);
        int board_row = row + next_top_row;

        if (after_falling == 0) {
            continue;
        }

        if (board_row < 0 || board_row >= board -> height) {
            return true;
        }

        if ((board -> arr[board_row] & after_falling) != 0) {
            return true;
        }
    }

    return false;
}

static bool rotation_collision_sim(const Board* board, const Tetrimino* tetrimino, Tetrimino_State next_rotation) {

    Tetrimino tetrimino_copy = *(tetrimino);
    tetrimino_copy.tetrimino_state = next_rotation;

    const Rotation_mask* rotation = &(all_shape_Tetrimino_State_Map[tetrimino_copy.shape][tetrimino_copy.tetrimino_state]);

    if (board -> height < rotation -> height + tetrimino_copy.height_top) {
        return true;
    }

    if (tetrimino_copy.x_right + rotation -> width > VISIBLE_WIDTH) {
        return true;
    }

    if (tetrimino_copy.x_right < 0) {
        return true;
    }

    for (uint8_t row = 0; row < TETRIMINO_BOX_HEIGHT; row++) {
        uint16_t after_rotation = get_current_piece_row(&tetrimino_copy, row);
        uint8_t board_row = tetrimino_copy.height_top + row;

        if (after_rotation == 0) {
            continue;
        }

        if (board_row < 0 || board_row >= board->height) {
            return true;
        }

        if ((board->arr[board_row] & after_rotation) != 0) {
            return true;
        }
    }

    return false;
}

// here after getting all the possible x_positions of the tetrimino, we run the hard_fall_sim
// then we calculate the scores across each
// then get the best score out of them, and copy over the values of the board to the real one
// because we know it is a valid position

Board_n_score* score_board_given_shape_at_pos_rotation(const Board* board, const Shape shape, const uint8_t x_right, uint8_t height_top, Tetrimino_State tetrimino_state) {
    Tetrimino tetrimino = set_tetrimino(shape, x_right, height_top, tetrimino_state);

    Board* simulated_board = simulate_drop_hard_and_place(board, &tetrimino);

    uint32_t score_sim = score_board_after_line_clears(simulated_board);

    Board_n_score* result = calloc(1, sizeof(Board_n_score));
    *result = (Board_n_score){.board = simulated_board, .score_sim = score_sim};
    return result;
}

static uint8_t clear_full_lines_sim(Board* board) {
    uint16_t line_clear_mask = (1u <<  VISIBLE_WIDTH) - 1;

    uint8_t lines_cleared = 0;
    for (int8_t row = board -> height - 1; row >= 2; row--) {
        if (board -> arr[row] == line_clear_mask) {
            lines_cleared += 1;
        }
    }
    return lines_cleared;
}

// free the returned board in the main loop !!!
Board* find_best_board_for_shape_omp(
    const Board* board,
    Shape shape) {


    uint8_t position_count = VISIBLE_WIDTH;    
    uint8_t rotation_count = Rotated_thrice + 1;

    uint8_t possible_x_positions[VISIBLE_WIDTH];
    for (uint8_t i = 0; i < VISIBLE_WIDTH; i++) {
        possible_x_positions[i] = i;
    }

    uint8_t height_top = 0;

    unsigned long candidate_count = position_count * rotation_count;

    Board_n_score** simulated_boards_n_scores = calloc(1, sizeof(Board_n_score*) * candidate_count);
    bool* valid = calloc(1 , sizeof(bool) * candidate_count);

    int thread_count = omp_get_num_procs();

    #pragma omp parallel for num_threads(thread_count) schedule(static)
    for (unsigned long i = 0; i < candidate_count; i++) {

        unsigned long rotation_index = i / position_count;
        unsigned long position_index = i % position_count;

        Tetrimino_State tetrimino_state = (Tetrimino_State)rotation_index;

        Tetrimino tetrimino = set_tetrimino(shape, possible_x_positions[position_index], height_top, tetrimino_state);

        if (rotation_collision_sim(board, &tetrimino, tetrimino_state)) {
            valid[i] = false;
            continue;
        }

        simulated_boards_n_scores[i] = score_board_given_shape_at_pos_rotation(board, tetrimino.shape, tetrimino.x_right, tetrimino.height_top, tetrimino.tetrimino_state);
        valid[i] = true;
    }

    size_t best_index = candidate_count;
    uint32_t best_score = 0;

    for (size_t i = 0; i < candidate_count; i++) {
        if (!valid[i] || simulated_boards_n_scores[i] == NULL) {continue;}

        if (best_index == candidate_count || simulated_boards_n_scores[i] -> score_sim > best_score) {
            best_index = i;
            best_score = simulated_boards_n_scores[i] -> score_sim;
        }
    }

    Board* best_board = NULL;

    if (best_index != candidate_count) {
        best_board = calloc(1, sizeof(Board));
        *best_board = hard_copy_of_board(simulated_boards_n_scores[best_index] -> board);
    }

    for (unsigned long i = 0; i < candidate_count; i++) {
        if (simulated_boards_n_scores[i] == NULL) {continue;}
        if (simulated_boards_n_scores[i] ->board != NULL) {
            free_board_ptr(simulated_boards_n_scores[i] ->board);
        }
        free(simulated_boards_n_scores[i]);
    }


    free(simulated_boards_n_scores);
    free(valid);

    return best_board;
}

/*
    Use this if you want to directly copy the best simulated result
    into the real board.

    Returns true if a valid placement was found.
*/
bool apply_best_board_for_shape_omp(Board* real_board, Shape shape) {
  
    Board* best_board = find_best_board_for_shape_omp(real_board, shape);

    if (best_board == NULL) {
        return false;
    }

    for (uint8_t height = 0; height < real_board -> height; height++) {
        real_board ->arr[height] = best_board->arr[height];
    }

    free_board_ptr(best_board);

    return true;
}

static uint32_t score_board_after_line_clears(Board* board) {

    uint8_t cleared_lines = clear_full_lines_sim(board);
    uint16_t holes = count_holes(board);
    uint16_t collective_height = get_collective_height(board);
    uint8_t max_height = get_max_height(board);
    uint16_t bumpiness = get_bumpiness(board);

    int64_t score = 100000;

    score += (int64_t)cleared_lines * 1000;
    score -= (int64_t)holes * 800;
    score -= (int64_t)collective_height * 20;
    score -= (int64_t)max_height * 40;
    score -= (int64_t)bumpiness * 10;

    if (score < 0)
        return 0;


    return (uint32_t)score;
}

// for me a whole is a block that the algo cant get to
/*
    |   x | --> under this is a hole
    |  x  | --> under this is a hole also, because algo cant get to it
    |xx xx|
    |xxxxx|   
    ______
*/

static uint16_t count_holes(const Board* board) {
    uint16_t holes = 0;

    uint8_t width = VISIBLE_WIDTH;

    for (uint8_t col = 0; col < width; col++) {
        bool found_block_above = false;
        uint16_t col_mask = (uint16_t)(1u << col);

        for (uint8_t row = 0; row < board -> height; row++) {
            bool filled = (board -> arr[row] & col_mask) != 0;

            if (found_block_above && (!filled)) 
                holes++;
            if (filled)
                found_block_above = true;
        }
    }

    return holes;
}

static uint8_t get_column_height(const Board* board, uint8_t col) {
    uint16_t mask = (uint16_t)(1u << (col));

    for (uint8_t row = 0; row < board -> height; row++) {
        if ((board -> arr[row] & mask) != 0) {
            return board -> height - row;
        }
    }

    return 0;
}


static uint16_t get_collective_height(const Board* board) {
    uint16_t total = 0;

    for (uint8_t col = 0; col < VISIBLE_WIDTH; col++) {
        total += get_column_height(board, col);
    }

    return total;
}


static uint8_t get_max_height(const Board* board) {
    uint8_t max_height = 0;

    for (int col = 0; col < VISIBLE_WIDTH; col++) {
        uint8_t height = get_column_height(board, col);

        if (height > max_height) {max_height = height;}
    }

    return max_height;
}

// if one column is bigger then the previous more bumpy 
static uint16_t get_bumpiness(const Board* board) {
    uint16_t bumpiness = 0;

    uint8_t previous_height = get_column_height(board, 0);

    for (int col = 1; col < VISIBLE_WIDTH; col++) {
        uint8_t current_height = get_column_height(board, col);

        bumpiness += (previous_height > current_height) ? previous_height - current_height : current_height - previous_height;
        previous_height = current_height;
    }

    return bumpiness;
}

void free_board_ptr(Board* board) {
    free(board -> arr);
    free(board);
}
