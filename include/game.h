#ifndef __GAME_H__
#define __GAME_H__

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
#include "preferences.h"
#include "powerups.h"
#include "star.h"

// CHANGE INCLUDE STATEMENTS AND ONLY PUT FUNCTIONS CALLED IN MAIN FILE

body_t *make_doodle(vector_t center, rgb_color_t color, char *info);

body_t *make_enemy(vector_t center);

body_t *make_button(vector_t center);

void more_platforms(scene_t *scene, vector_t center, int powerup_timer);

void more_enemies(scene_t *scene, vector_t center);

scene_t *make_game_scene();

bool in_screen(vector_t center, body_t *body);

body_t *wrap(body_t *doodle);

body_t *make_pellet (vector_t center);

void create_star(scene_t *scene);

double calculate_score(vector_t center);

void game_mouse_click (scene_t *scene, int x, int y);

void game_main(scene_t *scene, body_t *doodle, int *star_timer, int *powerup_timer, int *timer, vector_t *center, char *score);

#endif // #ifndef __GAME_H__