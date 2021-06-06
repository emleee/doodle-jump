#ifndef __PLATFORMS_H__
#define __PLATFORMS_H__

#include "scene.h"

/**
 * Creates a platform with the given characteristics.
 * 
 * @param center the center of the platform
 * @param color the color
 * @param info the platform's info
 * @param width the platform width
 * @param height the platform height
 * @param sprite the platform's sprite
 * 
 * @return a newly allocated body for the platform
 */
body_t *make_platform(vector_t center, rgb_color_t color, char *info, int width, int height, sprite_t *sprite);

/**
 * Creates a normal platform.
 * 
 * @param center the platform's center
 * @param info the platform's info, whihc is "normal platform"
 */
body_t *normal_platform(vector_t center, char *info);

/**
 * Creates a sliding platform.
 * 
 * @param center the platform's center
 * @param info the platform's info, whihc is "sliding platform"
 */
body_t *sliding_platform(vector_t center, char *info);

/**
 * Adjusts a sliding platform if it goes off the screen.
 * 
 * @param body a pointer to a sliding platform
 */
void sliding_bounce(body_t *body);

/**
 * Creates a disappearing platform.
 * 
 * @param center the platform's center
 * @param info the platform's info, whihc is "disappearing platform"
 */
body_t *disappearing_platform(vector_t center, char *info);

/**
 * Creates a trick platform.
 * 
 * @param center the platform's center
 * @param info the platform's info, whihc is "trick platform"
 */
body_t *trick_platform(vector_t center, char *info);

#endif // #ifndef __PLATFORMS_H__