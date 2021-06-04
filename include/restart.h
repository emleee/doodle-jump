#include "game.h"

/**
 * Handles the mouse-clicking events in the restart screen for selecting options for restarting the game or
 * returning to the start screen. 
 *
 * @param scene a pointer to the current scene
 * @param x the x-coordinate of the mouse-click
 * @param y the y-coordinate of the mouse-click
 * @param button_x_radius the width of the button
 * @param button_y_radius the height of the button
 */
void restart_mouse_click(scene_t *scene, int x, int y, double button_x_radius, double button_y_radius);

/** 
 * Creates a scene for the restart screen.
 * 
 * @param score the score from the last run
 * @param highscore the high score
 * @return a newly allocated scene
 */
scene_t *make_restart_scene(char *score, char *highscore);