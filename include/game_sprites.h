#ifndef __GAME_SPRITES_H__
#define __GAME_SPRITES_H__

#include "scene.h"

/**
 * Creates a body representing Doodle with the specified parameters.
 *
 * @param center the vector representing where the body should be centered
 * @param info additional information to associate with the body,
 *   e.g. its type if the scene has multiple types of bodies
 * @param sprite the sprite associated with Doodle
 * @return a pointer to the body for Doodle
 */
body_t *make_doodle(vector_t center, char *info, sprite_t *sprite);

/**
 * Creates a body representing the background with the specified parameters.
 *
 * @param file the file for the PNG for that background
 * @param center the vector representing where the body should be centered
 * @return a pointer to the body for the background
 */
body_t *make_background_body(char *file, vector_t center);

/**
 * Changes the direction that Doodle is looking 
 *
 * @param body the body for Doodle
 * @param sprite the sprite for the direction he is looking
 */
void change_direction(body_t *body, sprite_t *sprite);

/**
 * Find the coordinates of Doodle's mouth (for shooting pellets)
 *
 * @param body the body for Doodle
 */
vector_t find_mouth(body_t *body);

/**
 * Find the coordinates of Doodle's hand (for holding the magnet)
 *
 * @param body the body for Doodle
 */
vector_t find_hand(scene_t *scene, body_t *body, body_t *powerup);

#endif // #ifndef __GAME_SPRITES__