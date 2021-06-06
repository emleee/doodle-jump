#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <SDL2/SDL_image.h>
#include "vector.h"

/**
 * A sprite used for graphics.
 * Sprites are usually associated with a body.
 */
typedef struct sprite sprite_t;

/**
 * Allocates memory for a sprite with the given info.
 * 
 * @param texture the texture from a PNG for the sprite
 * @param width the width of the sprite
 * @param height the height of the sprite
 * @return a pointer to the newly allocated sprite
 */
sprite_t *sprite_init(SDL_Texture *texture, int width, int height);

/**
 * Moves the center of the sprite to the given coordinates and updates the box
 * parameters accordingly
 * 
 * @param sprite the sprite to be moved
 * @param center the point to which the sprite should be moved
 */
void sprite_set_center(sprite_t *sprite, vector_t center);

/**
 * Gets the texture associated with the sprite for rendering purposes
 * 
 * @param sprite a pointer to the sprite returned from sprite_init()
 * @return the texture passed to sprite_init()
 */
SDL_Texture *sprite_get_texture(sprite_t *sprite);

/**
 * Gets the SDL_Rect associated with the sprite for rendering purposes
 * 
 * @param sprite a pointer to the sprite returned from sprite_init()
 * @return the SDL_Rect representing the sprite's position and dimensions
 */
SDL_Rect *sprite_get_box(sprite_t *sprite);

/**
 * Releases the memory allocated for a sprite.
 *
 * @param sprite a pointer to a sprite returned from sprite_init()
 */
void sprite_free(sprite_t *sprite);

#endif // #ifndef __SPRITE_H__
