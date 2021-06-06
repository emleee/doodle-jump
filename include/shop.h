#ifndef __SHOP_H__
#define __SHOP_H__

#include "scene.h"

/**
 * Creates the shop screen, with text for prices and star count, and images of the powerups.
 * Only called when the user selects the "Shop" option on the start screen.
 *
 * @return the shop scene
 */
scene_t *make_shop_scene ();

/**
 * Creates the temporary shop exit screen, with text that reads "Thank you for shopping with us!".
 * Only called when the user selects a powerup on the shop screen.
 *
 * @return the shop exit scene
 */
scene_t *make_shop_exit_scene();

scene_t *make_failed_purchase_scene();

/**
 * Handles the mouse-clicking events in the shop for selecting powerups to purchase. 
 *
 * @param scene the current scene
 * @param x the x-coordinate of the mouse-click
 * @param y the y-coordinate of the mouse-click
 * @param button_x_radius the width of the button
 * @param button_y_radius the height of the button
 */
void shop_mouse_click(scene_t *scene, int x, int y);

/**
 * Handles action when the player attempts to buy immunity. Will either buy the powerup and add
 * it to their inventory or inform the player that they can't buy the powerup.
 */
void buy_immunity(scene_t *scene);

/**
 * Handles action when the player attempts to buy a magnet. Will either buy the powerup and add
 * it to their inventory or inform the player that they can't buy the powerup.
 */
void buy_magnet(scene_t *scene);

/**
 * Handles action when the player attempts to buy a boost. Will either buy the powerup and add
 * it to their inventory or inform the player that they can't buy the powerup.
 */
void buy_boost(scene_t *scene);

/**
 * Returns the number of stars the player has.
 * 
 * @return the number of stars
 */
int get_star_count();

/**
 * Changes the numebr of stars to the given count.
 * 
 * @param new_count the number of stars to be changed to
 */
void change_star_count(int new_count);

/**
 * Saves the purchased powerup to a file.
 * 
 * @param powerup the purchased powerup
 */
void write_powerup(char *powerup);

/**
 * Implements powerup purchase in the next game such that the character starts off with the
 * previously purchased powerup. Clears inventory until next purchase.
 *
 * @param scene the current scene
 * @param x the x-coordinate of the mouse-click
 * @param y the y-coordinate of the mouse-click
 * @param button_x_radius the width of the button
 * @param button_y_radius the height of the button
 */
void use_inventory (scene_t *scene);

/**
 * Displays the number of stars the user has with a star in front.
 * 
 * @param scene the pointer to the shop scene
 */
void display_star_count(scene_t *scene);

#endif // #ifndef __SHOP_H__