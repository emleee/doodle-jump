#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Creates the settings screen, with options for the sounds, score marker, and powerups.
 * Only called when the user selects the "Settings" option on the start screen.
 *
 * @return the shop scene
 */
scene_t *make_settings_scene();

/**
 * Reads from "preferences.txt" to current sound preference (on or off).
 * 
 * @return whether or not the user wants to hear sound-effects
 */
bool get_sound_preference();

/**
 * Reads from "preferences.txt" to current score-marker preference (on or off).
 * 
 * @return whether or not the user wants to see the score marker while playing the game
 */
bool get_score_preference();

/**
 * Reads from "preferences.txt" to current powerup preference (on or off).
 * 
 * @return whether or not the user wants to have powerups appear during the game
 */
bool get_powerup_preference();

/**
 * Switches the sound preference from on to off, or vice versa.
 * Only called when the user selects the sound option on the settings screen.
 */
void switch_sound_preferences();

/**
 * Switches the score-marker preference from on to off, or vice versa.
 * Only called when the user selects the score option on the settings screen.
 */
void switch_score_preferences();

/**
 * Switches the powerup preference from on to off, or vice versa.
 * Only called when the user selects the powerup option on the settings screen.
 */
void switch_powerup_preferences();

/**
 * Writes to "preferences.txt" with updated preferences.
 * Only called when the user selects an option on the settings screen.
 */
void update_preferences();

/**
 * Handles the mouse-clicking events in settings for switching options. 
 *
 * @param scene a pointer to the current scene
 * @param x the x-coordinate of the mouse-click
 * @param y the y-coordinate of the mouse-click
 * @param button_x_radius the width of the button
 * @param button_y_radius the height of the button
 */
void settings_mouse_click (scene_t *scene, int x, int y, double button_x_radius, double button_y_radius);

#endif // #ifndef __PREFERENCES_H__