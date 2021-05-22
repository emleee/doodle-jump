#ifndef __GAME_SPRITES__
#define __GAME_SPRITES__

#include "sprite.h"
#include "vector.h"
#include "body.h"

const vector_t RIGHT_OFFSET;
const vector_t LEFT_OFFSET;

sprite_t *make_jump_left();

sprite_t *make_jump_right();

sprite_t *make_background();

void face_right(body_t *body, sprite_t *sprite);

void face_left(body_t *body, sprite_t *sprite);

#endif // #ifndef __GAME_SPRITES__