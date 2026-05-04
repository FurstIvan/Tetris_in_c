#ifndef TETRIS_ASSETS_H
#define TETRIS_ASSETS_H

#include "common.h"

typedef struct Assets Assets;

struct Assets
{
    TTF_Font* font;
};

Assets* font_init();
void font_free(Assets*);
SDL_Texture* create_text_texture(SDL_Renderer* ,TTF_Font* ,const char* ,SDL_Color ,int* ,int* );

#endif