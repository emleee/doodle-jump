#include "sprite.h"
#include "body.h"
#include "vector.h"
#include "sdl_wrapper.h"
#include "math.h"

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
    sprite_t *sprite = create_sprite("PNGs/Jump_Right.png");
    sprite_set_parameters(sprite, VEC_ZERO, 117, 207);
    return sprite;
}

sprite_t *make_crouch_left() {
    sprite_t *sprite = create_sprite("PNGs/Crouch_Left.png");
    sprite_set_parameters(sprite, VEC_ZERO, 165, 140);
    return sprite;
}

sprite_t *make_crouch_right() {
    sprite_t *sprite = create_sprite("PNGs/Crouch_Right.png");
    sprite_set_parameters(sprite, VEC_ZERO, 165, 140);
    return sprite;
}

sprite_t *make_grass_platform() {
    sprite_t *sprite = create_sprite("PNGs/Grass_Platform.png");
    sprite_set_parameters(sprite, VEC_ZERO, 146, 35);
    return sprite;
}

sprite_t *make_background() {
    sprite_t *sprite = create_sprite("PNGs/Background.png");
    sprite_set_parameters(sprite, VEC_ZERO, 720, 960);
    return sprite;
}

void change_direction(body_t *body, sprite_t *sprite) {
    body_set_sprite(body, sprite);
    if (body_get_direction(body) == 0) {
        body_set_centroid(body, vec_add(body_get_centroid(body), FACE_LEFT));
    }
    else {
        body_set_centroid(body, vec_add(body_get_centroid(body), FACE_RIGHT));
    }
}

void change_motion(body_t *body, sprite_t *sprite) {
    body_set_sprite(body, sprite);
    body_set_centroid(body, body_get_centroid(body));
}

vector_t find_mouth(body_t *body) {
    vector_t centroid = body_get_centroid(body);
    vector_t mouth;
    if (body_get_direction(body) == 0) {
        mouth = (vector_t){.x = centroid.x + 13, .y = centroid.y + 24};
    }
    else {
        mouth = (vector_t){.x = centroid.x - 13, .y = centroid.y + 24};
    }
    return mouth;
}