#include "sprite.h"
#include "body.h"
#include "vector.h"
#include "sdl_wrapper.h"

const rgb_color_t COLOR = {.r = 0, .g = 0, .b = 0};

const vector_t FACE_RIGHT = {.x = -25, .y = 0};
const vector_t FACE_LEFT = {.x = 25, .y = 0};

// sprite_t *LEFT_SPRITE;
// sprite_t *RIGHT_SPRITE;

const vector_t RIGHT_OFFSET = {.x = -12.5, .y = 33.5};
const vector_t LEFT_OFFSET = {.x = 12.5, .y = 33.5};

sprite_t *make_jump_left(vector_t center) {
    sprite_t *sprite = create_sprite("PNGs/Jump_Left.png");
    sprite_set_parameters(sprite, center, 134, 237);
    return sprite;
}

sprite_t *make_jump_right(vector_t center) {
    sprite_t *sprite = create_sprite("PNGs/Jump_RIGHT.png");
    sprite_set_parameters(sprite, center, 134, 237);
    return sprite;
}

void face_left(body_t *body, sprite_t *sprite) {
    sprite_set_center(sprite, vec_add(body_get_centroid(body), RIGHT_OFFSET));
    body_set_centroid(body, vec_add(body_get_centroid(body), FACE_LEFT));
    body_set_sprite(body, sprite);
}

void face_right(body_t *body, sprite_t *sprite) {
    sprite_set_center(sprite, vec_add(body_get_centroid(body), LEFT_OFFSET));
    body_set_centroid(body, vec_add(body_get_centroid(body), FACE_RIGHT));
    body_set_sprite(body, sprite);
}