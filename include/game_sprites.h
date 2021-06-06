#ifndef __GAME_SPRITES_H__
#define __GAME_SPRITES_H__

#include "scene.h"

body_t *make_doodle(vector_t center, rgb_color_t color, char *info, sprite_t *sprite);

sprite_t *make_grass_platform();

sprite_t *make_background();

body_t *make_background_body(char *file, vector_t center);

void change_direction(body_t *body, sprite_t *sprite);

void change_motion(body_t *body, sprite_t *sprite);

vector_t find_mouth(body_t *body);
vector_t find_hand(scene_t *scene, body_t *body, body_t *powerup);

#endif // #ifndef __GAME_SPRITES__