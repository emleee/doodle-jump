#ifndef __POWERUPS_H__
#define __POWERUPS_H__

#include <stddef.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "polygon.h"
#include "color.h"
#include "list.h"
#include <time.h>
#include <stdio.h>
#include "forces.h"
#include "collision.h"
#include "platforms.h"
#include "sdl_wrapper.h"
#include "game_sprites.h"
#include "SDL2/SDL_mixer.h"
#include "text.h"
#include "test_util.h"

bool is_powerup(body_t *body);
body_t *make_powerup(scene_t *scene);
body_t *make_boost(scene_t *scene, vector_t center);
body_t *make_immunity(scene_t *scene, vector_t center, bool collected);

body_t *make_magnet(scene_t *scene, vector_t center, bool collected);
void magnet_powerup(scene_t *scene, int *powerup_timer);

#endif // #ifndef __POWERUPS_H__
