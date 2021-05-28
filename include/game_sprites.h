#ifndef __GAME_SPRITES_H__
#define __GAME_SPRITES_H__

#include "sprite.h"
#include "vector.h"
#include "body.h"

const vector_t RIGHT_OFFSET;
const vector_t LEFT_OFFSET;

sprite_t *make_jump_left();

sprite_t *make_jump_right();

sprite_t *make_crouch_left();

sprite_t *make_crouch_right();

body_t *make_doodle(vector_t center, rgb_color_t color, char *info);

sprite_t *make_grass_platform();

sprite_t *make_background();

body_t *make_background_body(vector_t center);

void change_direction(body_t *body, sprite_t *sprite);

void change_motion(body_t *body, sprite_t *sprite);

vector_t find_mouth(body_t *body);

#endif // #ifndef __GAME_SPRITES__