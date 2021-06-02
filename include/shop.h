#include "game.h"
#include "restart.h"
#include "start.h"

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

/**
 * Handles the mouse-clicking events in the shop for selecting powerups to purchase. 
 *
 * @param scene the current scene
 * @param x the x-coordinate of the mouse-click
 * @param y the y-coordinate of the mouse-click
 * @param button_x_radius the width of the button
 * @param button_y_radius the height of the button
 */
void shop_mouse_click(scene_t *scene, int x, int y, double button_x_radius, double button_y_radius);

void buy_immunity();

void buy_magnet();

void buy_boost();

int get_star_count();

void change_star_count(int new_count);

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