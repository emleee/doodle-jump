#ifndef __GAME_H__
#define __GAME_H__

#include "platforms.h"
#include "preferences.h"
#include "powerups.h"

/**
 * Makes a body_t with a sprite representing an enemy.
 * 
 * @param center the position of the enemy's centroid
 * @return a pointer to a newly allocated body
*/
body_t *make_enemy(vector_t center);

/**
 * Makes a body_t in the shape of a circle.
 * 
 * @param center the position of the circle's centroid
 * @return a pointer to a newly allocated body
 */
body_t *make_button(vector_t center);

/**
 * Spawns more platforms out of the screen.
 * 
 * @param scene a pointer to the current scene
 * @param center the current center of the screen
 */
void more_platforms(scene_t *scene, vector_t center);

/**
 * Spawns more enemies out of the screen.
 * 
 * @param scene a pointer to the current scene
 * @param center the current center of the screen
 */
void more_enemies(scene_t *scene, vector_t center);

/**
 * Initializes a scene for the game with the doodle, platforms,
 * and enemies.
 * 
 * @return a pointer to a newly allocated scene
 */
scene_t *make_game_scene();


/**
 * Initializes a scene for the game instructions with different panels.
 * 
 * @return a pointer to a newly allocated scene
 */
scene_t *make_instructions_scene();

/**
 * Checks if a body has gone below the scene.
 * 
 * @param center the center of the scene
 * @param body the body to be checked
 * @return a boolean representing if the body is in the screen
 */
bool in_screen(vector_t center, body_t *body);

/**
 * Teleports the doodle if it goes off the screen horizontally.
 * 
 * @param doodle the body being teleported
 */
void wrap(body_t *doodle);

/**
 * Makes a body_t representing a pellet.
 * 
 * @param center the pellet's starting position
 * @return a pointer to a newly allocated body
*/
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

/**
 * Makes a body_t representing a star and adds it to the scene.
 * 
 * @param scene a pointer ot the current scene
*/
void create_star(scene_t *scene);

/**
 * Calculates the player's score based on the height.
 * 
 * @param center the center of the game scene
 * @return the score
 */
double calculate_score(vector_t center);

/**
 * Saves the number of stars collected to a file.
 * 
 * @param star_score the number of stars collected
 */
void star_updating(int star_score);

/**
 * If the new score is higher than the old high score, saves the new
 * score as the high score to a file.
 * 
 * @param score the new score
 */
void high_score_updating(char *score);

/**
 * Handles the mouse-clicking events in the game for shooting enemies.
 *
 * @param scene a pointer to the current scene
 * @param x the x-coordinate of the mouse-click
 * @param y the y-coordinate of the mouse-click
 */
void game_mouse_click (scene_t *scene, int x, int y);

/**
 * Handles the mouse-clicking events in the game instructions for toggling panels.
 *
 * @param scene a pointer to the current scene
 * @param x the x-coordinate of the mouse-click
 * @param y the y-coordinate of the mouse-click
 */
void instructions_mouse_click (scene_t *scene, int x, int y, double button_x_radius, double button_y_radius);

/*s*
 * Does the game.
 * 
 * @param scene the scene representing the game
 * @param doodle the doodle's body
 * @param star_timer a timer for generating stars
 * @param powerup_timer a timer for generating powerups
 * @param instructions_timer a timer for generating instructions
 * @param center the center of the scene
 * @param score the player's score
 */
void game_main (scene_t *scene, body_t *doodle, int *star_timer, int *powerup_timer, vector_t *center, char *score);


#endif // #ifndef __GAME_H__