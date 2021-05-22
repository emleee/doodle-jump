#include "sprite.h"
#include "body.h"
#include "vector.h"
#include "sdl_wrapper.h"

const vector_t RIGHT_OFFSET = {.x = 10.5, .y = 29.5};
const vector_t LEFT_OFFSET = {.x = -10.5, .y = 29.5};

const vector_t FACE_RIGHT = {.x = -21, .y = 0};
const vector_t FACE_LEFT = {.x = 21, .y = 0};

sprite_t *make_jump_left() {
    sprite_t *sprite = create_sprite("PNGs/Jump_Left.png");
    sprite_set_parameters(sprite, VEC_ZERO, 117, 207);
    return sprite;
}

sprite_t *make_jump_right() {
    sprite_t *sprite = create_sprite("PNGs/Jump_RIGHT.png");
    sprite_set_parameters(sprite, VEC_ZERO, 117, 207);
    return sprite;
}

sprite_t *make_background() {
    sprite_t *sprite = create_sprite("PNGs/Background.png");
    sprite_set_parameters(sprite, VEC_ZERO, 720, 960);
    return sprite;
}

void face_left(body_t *body, sprite_t *sprite) {
    sprite_set_center(sprite, vec_add(body_get_centroid(body), RIGHT_OFFSET));
    body_set_sprite(body, sprite);
    body_set_centroid(body, vec_add(body_get_centroid(body), FACE_LEFT));
}

void face_right(body_t *body, sprite_t *sprite) {
    sprite_set_center(sprite, vec_add(body_get_centroid(body), LEFT_OFFSET));
    body_set_sprite(body, sprite);
    body_set_centroid(body, vec_add(body_get_centroid(body), FACE_RIGHT));
}
