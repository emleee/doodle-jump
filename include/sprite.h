#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <stdbool.h>
#include "vector.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

typedef struct sprite sprite_t;

sprite_t *sprite_init(SDL_Texture *texture);

void sprite_set_parameters(sprite_t *sprite, vector_t center, double width, double height);

void sprite_translate(sprite_t *sprite, vector_t displacement);

void sprite_set_center(sprite_t *sprite, vector_t center);

vector_t sprite_get_center(sprite_t *sprite);

SDL_Texture *sprite_get_texture(sprite_t *sprite);

SDL_Rect *sprite_get_box(sprite_t *sprite);

#endif // #ifndef __SPRITE_H__
