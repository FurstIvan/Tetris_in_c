#ifndef TETRIS_SDL_APP_H
#define TETRIS_SDL_APP_H

#include "types.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

void game_loop(Tetris*);
double get_time(void);
bool SDL_init();

#endif