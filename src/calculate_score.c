// something else to keep in mind is that we could run each board on a different thread if we can so that way we achieve multi threading within the game

// then after we get the desired board we render it ... plus we should be able to tell how far into the future to look


// one thread will have the task of doing one x_pos and all the rotation types fast_drop and then look at the next shape and there maybe create new threads and make some new


// kinda like this:


/*

                *       first it has the task of row (1) for thread (1) -> hard drop it and the eval the position
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


/* 

    first im going to calculate all the boards untill depth of SHAPE_QUEUE_LEN

    then i will score them

    then i will choose the highest scored board out of them

    then i will take one step in that bundle of boards that i have calculated and save it and give it to main to show

    then i will free all other board trees and the one that i have made the step in the first one

    then i will calculate one more step so that im always maximum shapes ahead

    then go back to step scoring


    struct board_tree --> int amount of child, Board* boards that are accesible from here , Board* the board that we go from, points of that board space, 
                      --> something to traverse this tree with 

    struct Board_tree_n_point --> Board_tree* board_tree
                                  int point --> the best point that the tree produced

    from this we can get the best board if we have a list of all the possible next moves and to make a trunk out of it then fill up the tree untill nex tmove 

                    

    --> for this i need to implement, creating a trunk, adding nodes to a tree, deleting nodes from the tree 

                                  
    for the fillup algorithm i want to do it dynamic with shape queue lenght

    so im thinking like this, we start with a board. then we evaluate for all the positions inside that baord. then if we are not unitll shape_queue_len, we go deeper, untill we get there



*/


#include "calculate_score.h"

// going to calculate the state of the board given a state of it with a tetrimino not set in a place !!! this is very important
// i will use drophard without actually setting the real piece just a copy of it
// also the piece should be set at a determined x coordinate, i will not s


static Board_Tree_Node* create_board_tree_node(Board_n_score, uint8_t, Board_Tree_Node*, tetris_move);
static bool apply_best_move_and_advance_but_without_queue_update_and_extension_to_full_depth_with_extend_baord_tree_to_full_depth(Board_tree*, Board*);
static void expand_tree_to_depth_recursive(Board_Tree_Node*, const Queue*, uint8_t);
static Board hard_copy_of_board(const Board*);
static bool fall_collision_sim(const Board*, const Tetrimino*, int16_t);
static bool rotation_collision_sim(const Board*, const Tetrimino*, Tetrimino_State);
static uint8_t clear_full_lines_sim(Board*);
static uint32_t score_board_after_line_clears(Board_n_score*);
static uint16_t count_holes(const Board*);
static uint8_t get_column_height(const Board*, uint8_t);
static uint16_t get_all_height(const Board*);
static uint8_t get_max_height(const Board*);
static uint16_t get_bumpiness(const Board*);
static Board_tree_queue* create_queue_board_tree(void);
static void free_board_ptr(Board*);
static void free_queue_board_tree(Board_tree_queue*);
static void add_queue_board_tree(Board_tree_queue*, Board_Tree_Node*);
static Board_Tree_Node* delete_queue_board_tree (Board_tree_queue*);
static Board* copy_board_ptr (const Board*);
static bool add_Child(Board_Tree_Node* , Board_Tree_Node*);
static void generate_children_for_node(Board_Tree_Node* , const Queue* );
static Board_n_score* score_board_given_shape_at_pos_rotation(const Board*, const Shape, const uint8_t, uint8_t, Tetrimino_State) ;
static void line_clear_sim(Board*, int) ;
static void expand_tree_one_more(Board_Tree_Node*, const Queue* , uint8_t );
static Board* simulate_drop_hard_and_place(const Board*, const Tetrimino* );


Board* copy_board_ptr (const Board* src) {
    Board* board_ptr = malloc(sizeof(Board));

    *board_ptr = hard_copy_of_board(src);
    return board_ptr;
}

Board_Tree_Node* delete_queue_board_tree (Board_tree_queue* queue_ptr) {
    queue_ptr -> beg++;
    return queue_ptr->queue_of_tree_nodes[queue_ptr -> beg - 1]; // because we made beg -1 before and this is the true beg
}

void add_queue_board_tree(Board_tree_queue* queue_ptr, Board_Tree_Node* board_tree_node) {
    queue_ptr->queue_of_tree_nodes[queue_ptr -> end] = board_tree_node;
    queue_ptr->end++;
    return;
}

void free_queue_board_tree(Board_tree_queue* queue_ptr) {
    free(queue_ptr);
}

Board_tree_queue* create_queue_board_tree(void) {
    Board_tree_queue* queue = malloc(sizeof(Board_tree_queue));
    queue -> end = 0;
    queue -> beg = 0;
    queue -> queue_of_tree_nodes = NULL;

    return queue;
}

Board_Tree_Node* create_board_tree_node(Board_n_score board_n_score, uint8_t depth, Board_Tree_Node* parent, tetris_move move) {
    Board_Tree_Node* tree_node = malloc(sizeof(Board_Tree_Node));

    tree_node -> this_node = board_n_score;
    tree_node -> parent = parent;

    tree_node -> link_count = 0;
    tree_node -> tree_depth = depth;

    tree_node -> move = move;

    tree_node -> best_score = 0;
    tree_node -> best_link = NULL;

    tree_node -> expanded = false;
    return tree_node;
}

bool add_Child(Board_Tree_Node* parent, Board_Tree_Node* child) {
    if (parent -> link_count >= MAX_LINK) {
        return false;
    }

    parent -> link_Nodes[parent -> link_count] = child;
    parent -> link_count++;

    parent -> expanded = true;

    return true;
}


void delete_tree_recursive(Board_Tree_Node* root) {
    for (uint8_t i = 0; i < root -> link_count; i++) {
        delete_tree_recursive(root -> link_Nodes[i]);
    }

    if (root->this_node.board != NULL) {
        free_board_ptr(root->this_node.board);
    }

    free(root);
}

// this simulates 1 layer
void generate_children_for_node(Board_Tree_Node* parent, const Queue* shape_queue) {
    if (parent -> expanded) { // there are already kids inside
        return;
    }

    if (parent->tree_depth == FUTURE_SIGHT) { // we already reached max depth
        return;
    }
    
    if (parent -> this_node.score_sim < 1000 && parent -> tree_depth > 0) { // after this point maybe there is no reason to look further so we just stop
        parent -> this_node.score_sim = 0;
        return;
    }
    
    Shape current_shape = shape_queue -> shapes[(shape_queue->front + parent -> tree_depth) % shape_queue -> lenght];

    const uint8_t position_count = VISIBLE_WIDTH;
    const uint8_t rotation_count = (uint8_t)Rotated_thrice + 1; // downcast !!

    for (uint8_t i = 0; i < position_count * rotation_count; i++) {
        uint8_t current_pos = i % position_count;
        uint8_t current_rotation = i / position_count;
        
        Tetrimino_State current_tetrimino_state = (Tetrimino_State)current_rotation;

        // always max height so we just give it 0
        Tetrimino tetrimino = set_tetrimino(current_shape, current_pos, 0, current_rotation);

        tetris_move move = {.shape = current_shape, .rotation = current_rotation, .x_right = current_pos};

        if (rotation_collision_sim(parent->this_node.board, &tetrimino, current_rotation)) {
            continue;
        } 

        Board* simulated_board = simulate_drop_hard_and_place(parent -> this_node.board, &tetrimino);

        Board_n_score child_board_n_score = {.board = simulated_board, .lines_cleared = 0, .score_sim = 0};

        child_board_n_score.score_sim = score_board_after_line_clears(&child_board_n_score);

        Board_Tree_Node* child = create_board_tree_node(child_board_n_score, parent -> tree_depth + 1, parent, move);

        if (!add_Child(parent, child)) {
            delete_tree_recursive(child);
        }
    }
   

    parent -> expanded = true;

    return;
}

// untill maxdepth we keep expanding the tree
void expand_tree_to_depth_recursive(Board_Tree_Node* root, const Queue* shape_queue, uint8_t max_Depth) {
    if (root -> tree_depth == max_Depth) return;

    generate_children_for_node(root, shape_queue);

    if (PARALLEL_ON) {
        #pragma omp taskgroup 
        {
            for (uint8_t child_count = 0; child_count < root -> link_count; child_count++) {

                Board_Tree_Node* child = root ->link_Nodes[child_count];
                #pragma omp task firstprivate(child) shared(shape_queue, max_Depth)
                {
                    expand_tree_to_depth_recursive(child, shape_queue, max_Depth);
                }
            }
        }
    }
    else {
        for (uint8_t child_count = 0; child_count < root -> link_count; child_count++) {
            Board_Tree_Node* child = root ->link_Nodes[child_count];
            expand_tree_to_depth_recursive(root ->link_Nodes[child_count], shape_queue, max_Depth);
        }
    }
}

void expend_tree_once(Board_Tree_Node* root, const Queue* shape_queue, uint8_t max_Depth) {
    if (root -> tree_depth == max_Depth) return;

    generate_children_for_node(root, shape_queue);

    if (PARALLEL_ON) {
        #pragma omp parallel for 
        {
            for (uint8_t child_count = 0; child_count < root -> link_count; child_count++) {
                Board_Tree_Node* child = root ->link_Nodes[child_count];
            }
        }
    }
    else {
        for (uint8_t child_count = 0; child_count < root -> link_count; child_count++) {
            Board_Tree_Node* child = root ->link_Nodes[child_count];
        }
    }
}

uint32_t compute_best_subtree(Board_Tree_Node* root) {
    if (!(root -> expanded)) {
        return root->this_node.score_sim;
    }

    root -> best_score = 0;
    root -> best_link = NULL;

    for (uint8_t i = 0; i < root -> link_count; i++) {

        uint32_t child_score = compute_best_subtree(root->link_Nodes[i]);

        if (child_score > root->best_score) {
            root->best_link = root -> link_Nodes[i];
            root->best_score = child_score;
        }
    }
   
    if (root -> best_link == NULL) {
        root->best_link = root->link_Nodes[0];;
        return 0;
    }

    return root->best_score;
}


Board_tree* create_board_tree_from_current_board(const Board* current_board, const Queue* queue) {
    Board_tree* tree = malloc(sizeof(Board_tree));

    Board* trunk_board = copy_board_ptr(current_board);
    Board_n_score trunk_board_n_score = {.board = trunk_board, .score_sim = 0};
    tree -> trunk = create_board_tree_node(trunk_board_n_score, 0, 0, (tetris_move){0});
    
    // we expand than we calculate ez

    #pragma omp parallel 
    { 
        #pragma omp single
        {
            expand_tree_to_depth_recursive(tree->trunk, queue, FUTURE_SIGHT);
        }
    }
    compute_best_subtree(tree->trunk);

    return tree;
}

void copy_from_board_to_board (Board* destination, const Board* source) {
    for (uint8_t row = 0; row < destination->height; row++) {
        destination -> arr[row] = source -> arr[row];
    }
    return;
}

void recalculcate_depths(Board_Tree_Node* root, uint8_t new_Depth) {
    root -> tree_depth = new_Depth;

    for (uint8_t i = 0; i < root ->link_count; i++) {
       
        recalculcate_depths(root->link_Nodes[i], new_Depth+1);
    }

    return;
}

bool promote_best_child_to_root(Board_tree* tree) {
    Board_Tree_Node* old_trunk = tree -> trunk;
    Board_Tree_Node* chosen_one = old_trunk->best_link;

    if (chosen_one == NULL) {
        delete_tree_recursive(old_trunk);
        return false;
    }

    for (uint8_t i = 0; i < old_trunk -> link_count; i++) {
        if (old_trunk->link_Nodes[i] != chosen_one) {
            delete_tree_recursive(old_trunk->link_Nodes[i]);
        }
    } 

    free_board_ptr(old_trunk->this_node.board);
    free(old_trunk);

    chosen_one -> parent = NULL;
    tree->trunk = chosen_one;

    return true;
}

void expand_tree_one_more(Board_Tree_Node* root, const Queue* queue, uint8_t max_depth) {
    if (!root ->expanded && root -> tree_depth == max_depth - 1) { // we are in the last okay position
        expend_tree_once(root,queue,max_depth);
    }
    for (uint8_t i = 0; i < root ->link_count; i++) {
        expand_tree_one_more(root ->link_Nodes[i], queue, max_depth);
    }
}

void extend_board_tree_to_full_depth (Board_tree* tree, const Queue* queue) {
    expand_tree_one_more(tree ->trunk, queue, FUTURE_SIGHT);
    compute_best_subtree(tree->trunk);
    return;
}

bool apply_best_move_and_advance_but_without_queue_update_and_extension_to_full_depth_with_extend_baord_tree_to_full_depth(Board_tree* tree, Board* real_board) {
    compute_best_subtree(tree->trunk);

    Board_Tree_Node* chosen_one = tree -> trunk -> best_link;

    copy_from_board_to_board(real_board, chosen_one->this_node.board);
    if (!promote_best_child_to_root(tree)) {
        return false;
    };
    return true;    
}

// make sure that it acutally clears the lines here
Board* simulate_drop_hard_and_place(const Board* board, const Tetrimino* tetrimino) { // free it after use of board

    Board* copy_board = copy_board_ptr(board);

    Tetrimino copy_tetrimino = *(tetrimino);

    uint8_t height_top_new = (uint8_t)tetrimino -> height_top;

    while (!fall_collision_sim(copy_board, &copy_tetrimino, height_top_new)) {
        height_top_new += 1;
    }

    copy_tetrimino.height_top = height_top_new - 1; // we subtract one because we find collision one below so we go up one

    for (int row = 0; row < TETRIMINO_BOX_HEIGHT; row++) {
        uint16_t after_falling = get_current_piece_row(&copy_tetrimino, row);

        if (after_falling == 0) {
            continue;
        }

        uint8_t board_row = copy_tetrimino.height_top + row;

        copy_board->arr[board_row] |= after_falling;
    }

    return copy_board;
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
    
    if (rotation_collision_sim(board, &tetrimino, tetrimino_state)) {
        return NULL;
    }

    Board* simulated_board = simulate_drop_hard_and_place(board, &tetrimino);

    Board_n_score simulated_board_n_score = {.board = simulated_board, .lines_cleared = 0, .score_sim = 0};

    uint32_t score_sim = score_board_after_line_clears(&simulated_board_n_score);

    Board_n_score* result = calloc(1, sizeof(Board_n_score));
    *result = (Board_n_score){.board = simulated_board, .score_sim = score_sim};
    return result;
}


void line_clear_sim(Board* board, int row) {
    board -> arr[row] = 0;
    for (; row > 1; row--) {
        board -> arr[row] = board -> arr[row - 1];
    }
}

static uint8_t clear_full_lines_sim(Board* board) {
    uint16_t line_clear_mask = (1u <<  VISIBLE_WIDTH) - 1;

    uint8_t lines_cleared = 0;
    for (uint8_t row = board->height - 1; row >= 2; row--) {
        if (board -> arr[row] == line_clear_mask) {
            line_clear_sim(board, row);
            lines_cleared += 1;
            row += 1;
        }
    }
    return lines_cleared;
}

/*
    Use this if you want to directly copy the best simulated result
    into the real board.

    Returns true if a valid placement was found.
*/

static uint32_t score_board_after_line_clears(Board_n_score* board_n_score) {
    Board* board = board_n_score->board;

    uint8_t cleared_lines = clear_full_lines_sim(board);
    board_n_score->lines_cleared = cleared_lines;

    uint16_t holes = count_holes(board);
    uint16_t collective_height = get_all_height(board);
    uint8_t max_height = get_max_height(board);
    uint16_t bumpiness = get_bumpiness(board);

    int64_t score = 50000;

    score += (int64_t)cleared_lines * 1500;
    score -= (int64_t)holes * 900;
    score -= (int64_t)collective_height * 20; // on basic setting 10 * 20 
    score -= (int64_t)max_height * 90; // max 20
    score -= (int64_t)bumpiness * 10;

    if (score < 0)
        return 0;


    return (uint32_t)score;
}

// for me a hole is a block that the algo cant get to
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


static uint16_t get_all_height(const Board* board) {
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
