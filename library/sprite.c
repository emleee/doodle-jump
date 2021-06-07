#include "sprite.h"

typedef struct sprite {
    SDL_Texture *texture;
    vector_t center;
    SDL_Rect *box;
} sprite_t;

sprite_t *sprite_init(SDL_Texture *texture, int width, int height) {
    sprite_t *sprite = malloc(sizeof(sprite_t));
    sprite->texture = texture;

    sprite->box = malloc(sizeof(SDL_Rect));
    *(sprite->box) = (SDL_Rect) {.x = -width/2, .y = -height/2, .h = height, .w = width};
    sprite->box->w = width;
    sprite->box->h = height;
    
    sprite->center = VEC_ZERO;
    return sprite;
}

void sprite_set_center(sprite_t *sprite, vector_t center) {
    sprite->center.x = center.x;
    sprite->center.y = center.y;
    sprite->box->x = sprite->center.x - (sprite->box->w)/2;
    sprite->box->y = sprite->center.y - (sprite->box->h)/2;
}

SDL_Texture *sprite_get_texture(sprite_t *sprite) {
    return sprite->texture;
}

SDL_Rect *sprite_get_box(sprite_t *sprite) {
    return sprite->box;
}

void sprite_free(sprite_t *sprite) {
    SDL_DestroyTexture(sprite->texture);
    free(sprite->box);
    free(sprite);
}