#ifndef TETRIS_RENDER_H
#define TETRIS_RENDER_H

#include "types.h"

void render_current_frame(SDL_Renderer*,  Tetris*);
void set_tetrimino_color(SDL_Renderer*, Shape);
void render_side_bar(SDL_Renderer*, Tetris*);
void render_hold_tetrimino(SDL_Renderer*, Tetris*);

#endif