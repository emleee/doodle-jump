#include "vector.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include "sprite.h"

typedef struct sprite {
    vector_t center;
    SDL_Rect *box;
    SDL_Texture *texture;
} sprite_t;

sprite_t *sprite_init(SDL_Texture *texture) {
    sprite_t *sprite = malloc(sizeof(sprite_t));
    sprite->texture = texture;
    sprite->box = malloc(sizeof(SDL_Rect));
    return sprite;
}

void sprite_set_parameters(sprite_t *sprite, vector_t center, double width, double height) {
    *(sprite->box) = (SDL_Rect) {.x = center.x-width/2, .y = center.y-height/2, .h = height, .w = width};
    sprite->box->w = width;
    sprite->box->h = height;
    sprite->center = center;
}

void sprite_translate(sprite_t *sprite, vector_t displacement) {
    sprite->box->x += displacement.x;
    sprite->box->y -= displacement.y;
    sprite->center.x += displacement.x;
    sprite->center.y -= displacement.y;
}

void sprite_set_center(sprite_t *sprite, vector_t center) {
    sprite->center.x = center.x;
    sprite->center.y = 960-center.y;
    sprite->box->x = sprite->center.x - (sprite->box->w)/2;
    sprite->box->y = sprite->center.y - (sprite->box->h)/2;
}

vector_t sprite_get_center(sprite_t *sprite) {
    return sprite->center;
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