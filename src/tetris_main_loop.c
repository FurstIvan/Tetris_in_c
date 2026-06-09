#include "tetris_main_loop.h"
#include "data.h"
#include "input.h"
#include "render.h"
#include "game.h"
#include "calculate_score.h"


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

when tetrimino drops:
    change the rendering on of the tetriminos
    otherwise keep it
*/

// implement adding to score at eacch line clear
// better sdl3 visuals with textures instead of fill with collor


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

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

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize TTF: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    SDL_SetRenderLogicalPresentation(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    return true;
}


void game_loop(Tetris *tetris)
{
        
    omp_set_num_threads(omp_get_max_threads());
    
    Board_tree* trunk = create_board_tree_from_current_board(tetris->locked_board,tetris -> tetrimino_shape_queue);

    uint64_t last_score = 0;

    SDL_Texture* score_texture = render_score_texture_creater(renderer,tetris,tetris -> asset);

    double algo_speed = ALGO_SPEED;

    double last_algo_step = get_time();   

    double time_per_step_running_average = 0.0;
    size_t counter = 0;

    double start_of_algorithm = 0;

    while (tetris -> gs == INGAME) {

        double now = get_time();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                tetris -> gs = LOST;
            }
        }

        if (now - last_algo_step >= algo_speed) {

            start_of_algorithm = get_time();

            if (promote_best_child_to_root(trunk)) {
                get_new_tetrimino(tetris);
                copy_from_board_to_board(tetris->locked_board, trunk->trunk->this_node.board);
                recalculcate_depths(trunk->trunk, 0);
                extend_board_tree_to_full_depth(trunk, tetris->tetrimino_shape_queue);

                tetris -> score +=  (trunk ->trunk ->this_node.lines_cleared) * 800;

                if (last_score != tetris -> score) {
                    SDL_DestroyTexture(score_texture);

                    score_texture = render_score_texture_creater(renderer, tetris, tetris -> asset);

                    if (score_texture == NULL) {
                        SDL_Log("Failed to create score texture: %s", SDL_GetError());
                        exit(1);
                    }

                    last_score = tetris -> score;
                }
    
            } 
            else {
                tetris -> gs = LOST;
            }

            last_algo_step = now;
        }

        double finish = get_time();

        
        SDL_FRect text_destination = {
            .x = SCORE_TEXT_X - (score_texture -> w / 2),
            .y = SCORE_TEXT_Y,
            .w = score_texture -> w,
            .h = score_texture -> h,
        };

        SDL_SetRenderDrawColor(renderer,DEFAULT_COLOR,SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        render_current_frame(renderer, tetris); 
        render_side_bar(renderer,tetris);

        SDL_RenderTexture(renderer, score_texture, NULL, &text_destination);

        SDL_RenderPresent(renderer);

        double frame_end = get_time();

        counter++;
        time_per_step_running_average += ( (finish - start_of_algorithm) - time_per_step_running_average) / counter;
    }
    fprintf(stdout, "running avg: %lf" , time_per_step_running_average);
    if (tetris -> gs == LOST) { return; } 
    if (tetris -> gs == WON) { return; } 

    exit(1); // you either lose or win but somehow got something else just quit
    // nyerés implementálás --> survive x pontig
}



double get_time(void) {
    return (double)SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();
}