#ifndef __POWERUPS_H__
#define __POWERUPS_H__

#include "platforms.h"
#include "sdl_wrapper.h"
#include "game_sprites.h"
#include "test_util.h"

bool is_powerup(body_t *body);
body_t *make_powerup(scene_t *scene);
body_t *make_boost(scene_t *scene, vector_t center);
body_t *make_immunity(scene_t *scene, vector_t center, bool collected);

body_t *make_magnet(scene_t *scene, vector_t center, bool collected);
void immunity_powerup(scene_t *scene, int *powerup_timer);
void magnet_powerup(scene_t *scene, int *powerup_timer);

#endif // #ifndef __POWERUPS_H__
