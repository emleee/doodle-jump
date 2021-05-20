#ifndef __GAME_SPRITES__
#define __GAME_SPRITES__

#include "sprite.h"
#include "vector.h"
#include "body.h"

const vector_t RIGHT_OFFSET;
const vector_t LEFT_OFFSET;

sprite_t *make_jump_left(vector_t center);

sprite_t *make_jump_right(vector_t center);

void face_right(body_t *body, sprite_t *sprite);

void face_left(body_t *body, sprite_t *sprite);

#endif // #ifndef __GAME_SPRITES__