#include "sprite.h"
#include "body.h"
#include "vector.h"
#include "sdl_wrapper.h"
#include "math.h"

const vector_t RIGHT_OFFSET = {.x = 10.5, .y = 29.5};
const vector_t LEFT_OFFSET = {.x = -10.5, .y = 29.5};

const vector_t FACE_RIGHT = {.x = -21, .y = 0};
const vector_t FACE_LEFT = {.x = 21, .y = 0};

const rgb_color_t COLOR = {.r = 176.0/255, .g = 128.0/255, .b = 124.0/255};

const double DOODLE_WIDTH = 96.0;
const double DOODLE_HEIGHT = 148.0;

// sprite_t *make_jump_left() {
//     sprite_t *sprite = create_sprite("PNGs/Jump_Left.png", 117, 207);
//     sprite_set_parameters(sprite, VEC_ZERO, 117, 207);
//     return sprite;
// }

// sprite_t *make_jump_right() {
//     sprite_t *sprite = create_sprite("PNGs/Jump_Right.png", 117, 207);
//     sprite_set_parameters(sprite, VEC_ZERO, 117, 207);
//     return sprite;
// }

// sprite_t *make_crouch_left() {
//     sprite_t *sprite = create_sprite("PNGs/Crouch_Left.png", 165, 140);
//     sprite_set_parameters(sprite, VEC_ZERO, 165, 140);
//     return sprite;
// }

// sprite_t *make_crouch_right() {
//     sprite_t *sprite = create_sprite("PNGs/Crouch_Right.png", 165, 140);
//     sprite_set_parameters(sprite, VEC_ZERO, 165, 140);
//     return sprite;
// }

body_t *make_doodle(vector_t center, rgb_color_t color, char *info) {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {0, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {DOODLE_WIDTH, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {DOODLE_WIDTH, DOODLE_HEIGHT};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {0, DOODLE_HEIGHT};
    list_add(shape, v);

    sprite_t *doodle_sprite = create_sprite("PNGs/Jump_Right.png", 117, 207);
    body_t *doodle = body_init_with_sprite(shape, 10, color, info, free, doodle_sprite);
    body_set_centroid(doodle, center);

    return doodle;
}

// sprite_t *make_grass_platform() {
    // sprite_t *sprite = create_sprite("PNGs/Grass_Platform.png", 146, 35);
    // sprite_set_parameters(sprite, VEC_ZERO, 146, 35);
    // return sprite;
// }

// sprite_t *make_flower_platform() {
//     sprite_t *sprite = create_sprite("PNGs/Flower_Platform.png", 146, 35);
//     sprite_set_parameters(sprite, VEC_ZERO, 146, 35);
//     return sprite;
// }

sprite_t *make_background() {
    sprite_t *sprite = create_sprite("PNGs/Background.png", 720, 960);
    // sprite_set_parameters(sprite, VEC_ZERO, 720, 960);
    return sprite;
}

body_t *make_background_body(vector_t center) {
    char *info = malloc(11*sizeof(char));
    strcpy(info, "background");

    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {-1, 1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {1, 1};   // magic numbers
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {-1, -1};
    list_add(shape, v);

    sprite_t *sprite = make_background();
    body_t *background = body_init_with_sprite(shape, 1, COLOR, info, free, sprite);
    body_set_centroid(background, center);
    return background;
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