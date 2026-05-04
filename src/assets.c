#include "assets.h"

Assets* font_init(void) {
    Assets *asset = malloc(sizeof(Assets));

    asset->font = TTF_OpenFont("../assets/fonts/Creepster-Regular.ttf", 50.0f);

    if (asset->font == NULL) {
        SDL_Log("Failed to open font: %s", SDL_GetError());
        free(asset);
        exit(1);
    }

    return asset;
}

SDL_Texture* create_text_texture(
    SDL_Renderer* renderer,
    TTF_Font* font,
    const char* text,
    SDL_Color color,
    int* out_w,
    int* out_h
) {
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, 0, color);

    if (surface == NULL) {
        SDL_Log("Could not render text: %s", SDL_GetError());
        exit(1);
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL) {
        SDL_Log("Could not create texture from text surface: %s", SDL_GetError());
        exit(1);
        SDL_DestroySurface(surface);
        return NULL;
    }

    if (out_w != NULL) {
        *out_w = surface->w;
    }

    if (out_h != NULL) {
        *out_h = surface->h;
    }

    SDL_DestroySurface(surface);
    return texture;
}

void font_free(Assets* asset) {

    free(asset);

    return;
}