#include "game.h"
#include "sdl_app.h"

int main (void) {
    if (!SDL_init()) {
        return 1;
    }

    Tetris* tetris = init_game();

    game_loop(tetris);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(tetris -> asset-> font);
    TTF_Quit();
    SDL_Quit();

    free_tetris(tetris);

    return 0;
}