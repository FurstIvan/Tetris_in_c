#include "main.h"

int main(void) {
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