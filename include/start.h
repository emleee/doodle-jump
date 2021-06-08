#ifndef __START_H__
#define __START_H__

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
#include "game.h"

/**
 * Creates the start screen, with options for starting the game and going to settings and the shop.
 * Only called when the user selects the "Shop" option on the start screen.
 *
 * @return the start scene
 */
scene_t *make_start_scene();

/**
 * Handles the mouse-clicking events in the start screen for selecting options for the game, settings, or shop. 
 *
 * @param scene a pointer to the current scene
 * @param x the x-coordinate of the mouse-click
 * @param y the y-coordinate of the mouse-click
 * @param button_x_radius the width of the button
 * @param button_y_radius the height of the button
 */
void start_mouse_click(scene_t *scene, int x, int y, double button_x_radius, double button_y_radius);

/**
 * Calls the above two methods to create the start screen interface.
 * 
 * @param scene a pointer to the start screen scene
 * @param timer the timer for displaying graphics
 */
void start_main(scene_t *scene, int timer);

#endif // #ifndef __START_H__