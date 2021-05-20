#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "color.h"
#include "vector.h"

// TEXT RENDERING - MOVE TO OWN FILE?
typedef struct text {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect *textbox;
} text_t;

text_t *text_create(SDL_Renderer *renderer, char *string, rgb_color_t fontColor, int ptsize, vector_t *center, double width, double height) {
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("DoodleJump.ttf", ptsize);
    SDL_Color color = {fontColor.r, fontColor.g, fontColor.b};
    SDL_Surface *surface = TTF_RenderText_Solid(font, string, color);
    // SDL_Texture *texture = malloc(sizeof(SDL_Texture));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect *textbox = NULL;
    textbox->x = center->x;
    textbox->y = center->y;
    textbox->w = width;
    textbox->h = height;

    text_t *text = malloc(sizeof(text_t));
    text->surface = surface;
    text->texture = texture;
    text->textbox = textbox;

    TTF_CloseFont(font);
    return text;
}

void text_free(text_t *text) {
    SDL_FreeSurface(text->surface);
    SDL_DestroyTexture(text->texture);
    free(text);
}

SDL_Surface *text_get_surface(text_t *text) {
    return text->surface;
}

SDL_Texture *text_get_texture(text_t *text) {
    return text->texture;
}

SDL_Rect *text_get_textbox(text_t *text) {
    return text->textbox;
}