// Different Screens
#include "game.h"
#include "restart.h"
#include "start.h"

scene_t *make_shop_scene ();

void shop_mouse_click(scene_t *scene, int x, int y, double button_x_radius, double button_y_radius);

void buy_immunity();

void buy_magnet();

void buy_boost();

int get_star_count();
void change_star_count(int new_count);
void write_powerup(char *powerup);
void use_inventory (scene_t *scene);