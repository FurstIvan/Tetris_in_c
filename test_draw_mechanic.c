#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;


bool SDL_init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("Tetris_in_C", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("couldnt create window and renderer: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    SDL_SetRenderLogicalPresentation(&renderer, 320, 180, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    return true;
}

void render_current_frame(SDL_Renderer* renderer) {
    SDL_Frect rect;


    return;
}

int main(void) {


    while (true)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                return 0;
            }
        }

        render_current_frame(renderer, tetris); // make the logic for the rendering here

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    return 0;
}



