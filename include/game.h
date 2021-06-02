#ifndef __GAME_H__
#define __GAME_H__


#include "platforms.h"
#include "preferences.h"
#include "powerups.h"

// CHANGE INCLUDE STATEMENTS AND ONLY PUT FUNCTIONS CALLED IN MAIN FILE

body_t *make_enemy(vector_t center);

body_t *make_button(vector_t center);

bool more_platforms(scene_t *scene, vector_t center, int powerup_timer);

void more_enemies(scene_t *scene, vector_t center);

scene_t *make_game_scene();

bool in_screen(vector_t center, body_t *body);

body_t *wrap(body_t *doodle);

body_t *make_pellet (vector_t center);

/**
 * Reads from "first_time.txt" to check whether or not this is the users first time playing the game.
 *
 * @return whether or not this is the users first time playing the game
 */
bool first_time_play ();

/**
 * Writes to "first_time.txt" to say that the user has played the game before and they no longer need
 * to see the game instructions.
 */
void set_first_time();

void create_star(scene_t *scene);

double calculate_score(vector_t center);

/**
 * Handles the mouse-clicking events in the game for shooting enemies. 
 *
 * @param scene the current scene
 * @param x the x-coordinate of the mouse-click
 * @param y the y-coordinate of the mouse-click
 */
void game_mouse_click (scene_t *scene, int x, int y);

void game_main (scene_t *scene, body_t *doodle, int *star_timer, int *powerup_timer, int *timer, int *instructions_timer, vector_t *center, char *score);


#endif // #ifndef __GAME_H__