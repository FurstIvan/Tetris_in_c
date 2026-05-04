#include "sdl_app.h"
#include "data.h"
#include "input.h"
#include "render.h"
#include "game.h"

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
    otherwise keep ti
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
    double previous_time = get_time();
    double delta_time = 0.0;

    uint8_t current_grav = 1;
    
    tetris -> grav = grav_ramp_states[current_grav];

    Input_types last_input = TRAP;

    uint64_t last_score = 0;

    SDL_Texture* score_texture = render_score_texture_creater(
        renderer,
        tetris,
        tetris->asset
    );

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

        if (last_score != tetris->score) {
            SDL_DestroyTexture(score_texture);

            score_texture = render_score_texture_creater(
                renderer,
                tetris,
                tetris -> asset
            );

            if (score_texture == NULL) {
                SDL_Log("Failed to create score texture: %s", SDL_GetError());
                exit(1);
            }

            last_score = tetris -> score;
        }

        
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
        render_hold_tetrimino(renderer, tetris);

        SDL_RenderTexture(renderer, score_texture, NULL, &text_destination);

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

double get_time(void) {
    return (double)SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();
}