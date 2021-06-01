// Different Screens
#include "game.h"
#include "restart.h"
#include "start.h"

scene_t *make_shop_scene ();

void *shop_mouse_click();

void buy_immunity();

void buy_magnet();

void buy_boost();

int get_star_count();
void change_star_count(int new_count);
void write_powerup(char *powerup);