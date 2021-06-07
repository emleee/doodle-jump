#include "text.h"

typedef struct text {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect *textbox;
} text_t;

text_t *text_init(SDL_Surface *surface, SDL_Texture *texture, SDL_Rect *textbox) {
    text_t *text = malloc(sizeof(text_t));
    text->surface = surface;
    text->texture = texture;
    text->textbox = textbox;
    return text;
}

void text_free(text_t *text) {
    SDL_FreeSurface(text->surface);
    SDL_DestroyTexture(text->texture);
    free(text->textbox);
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