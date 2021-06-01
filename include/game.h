#ifndef __GAME_H__
#define __GAME_H__


#include "platforms.h"
#include "preferences.h"
#include "powerups.h"

// CHANGE INCLUDE STATEMENTS AND ONLY PUT FUNCTIONS CALLED IN MAIN FILE

body_t *make_doodle(vector_t center, rgb_color_t color, char *info);

body_t *make_enemy(vector_t center);

body_t *make_button(vector_t center);

bool more_platforms(scene_t *scene, vector_t center, int powerup_timer);

void more_enemies(scene_t *scene, vector_t center);

scene_t *make_game_scene();

bool in_screen(vector_t center, body_t *body);

body_t *wrap(body_t *doodle);

body_t *make_pellet (vector_t center);

void create_star(scene_t *scene);

double calculate_score(vector_t center);

void game_mouse_click (scene_t *scene, int x, int y);

void game_main (scene_t *scene, body_t *doodle, int *star_timer, int *powerup_timer, int *timer, int *instructions_timer, vector_t *center, char *score);

bool first_time_play ();
void set_first_time();
#endif // #ifndef __GAME_H__