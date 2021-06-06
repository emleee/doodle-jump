#ifndef __POWERUPS_H__
#define __POWERUPS_H__

#include "scene.h"

/**
 * Checks whether or not a body is a powerup (i.e. boost, magnet, or immunity).
 *
 * @param body the body in a scene
 * @return whether or not this body is a powerup
 */
bool is_powerup(body_t *body);

/**
 * Generates a random powerup (i.e. boost, magnet, or immunity).
 *
 * @param scene the current scene
 * @param enemy_present whether or not an enemy is present in the current scene
 * @return the powerup body
 */
body_t *make_powerup(scene_t *scene, bool enemy_present);

/**
 * Generates a boost powerup and adds this to the current scene.
 *
 * @param scene the current scene
 * @param center the center of an upcoming empty platform
 * @return the powerup body
 */
body_t *make_boost(scene_t *scene, vector_t center);

/**
 * Generates a immunity powerup and adds this to the current scene.
 *
 * @param scene the current scene
 * @param center the center of an upcoming empty platform
 * @return the powerup body
 */
body_t *make_immunity(scene_t *scene, vector_t center, bool collected);

/**
 * Generates a magnet powerup and adds this to the current scene.
 *
 * @param scene the current scene
 * @param center the center of an upcoming empty platform
 * @return the powerup body
 */
body_t *make_magnet(scene_t *scene, vector_t center, bool collected);

/**
 * Equips the immunity powerup once collected and adds necessary forces between newly-generated enemies.
 *
 * @param scene the current scene
 * @param powerup_timer the timer between powerup generation
 */
void immunity_powerup(scene_t *scene, int *powerup_timer);

/**
 * Equips the magnet powerup once collected and adds necessary forces between newly-generated stars.
 *
 * @param scene the current scene
 * @param powerup_timer the timer between powerup generation
 */
void magnet_powerup(scene_t *scene, int *powerup_timer);

#endif // #ifndef __POWERUPS_H__
