#include "game.h"
#include "sdl_app.h"

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