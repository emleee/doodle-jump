#include "polygon.h"
#include "sdl_wrapper.h"
#include "game_sprites.h"

// direction change offsets
const vector_t FACE_RIGHT = {.x = -21, .y = 0};
const vector_t FACE_LEFT = {.x = 21, .y = 0};

// pellet color
const rgb_color_t COLOR = {.r = 176.0/255, .g = 128.0/255, .b = 124.0/255};

// doodle parameters
const double DOODLE_WIDTH = 96.0;
const double DOODLE_HEIGHT = 148.0;
const double DOODLE_MASS = 10;

// magnet stuff
const int RIGHT_MAGNET_IDX = 4;
const int LEFT_MAGNET_IDX = 5;
const vector_t HAND_OFFSET = {.x = 70, .y = 15};
const vector_t MOUTH_OFFSET = {.x = 13, .y = 24};

// background dimensions
const vector_t BACKGROUND_DIMENSIONS = {.x = 720, .y = 960};

body_t *make_doodle(vector_t center, rgb_color_t color, char *info, sprite_t *sprite) {
    list_t *shape = make_rectangle(VEC_ZERO, DOODLE_WIDTH, DOODLE_HEIGHT);

    char *second_info = malloc(5 * sizeof(char));
    strcpy(second_info, "jump");

    body_t *doodle = body_init_with_sprite(shape, DOODLE_MASS, color, info, free, sprite);
    body_set_centroid(doodle, center);
    body_set_second_info(doodle, second_info);

    return doodle;
}

body_t *make_background_body(char *file, vector_t center) {
    list_t *shape = make_rectangle((vector_t){.x = -1, .y = -1}, 2, 2); // come back to this to make constants
    
    char *info = malloc(11*sizeof(char));
    strcpy(info, "background");

    sprite_t *sprite = create_sprite(file, BACKGROUND_DIMENSIONS.x, BACKGROUND_DIMENSIONS.y);
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

vector_t find_mouth(body_t *body) {
    vector_t centroid = body_get_centroid(body);
    vector_t mouth;
    if (body_get_direction(body) == 0) {
        mouth = (vector_t){.x = centroid.x + MOUTH_OFFSET.x, .y = centroid.y + MOUTH_OFFSET.y};
    }
    else {
        mouth = (vector_t){.x = centroid.x - MOUTH_OFFSET.x, .y = centroid.y + MOUTH_OFFSET.y};
    }
    return mouth;
}

vector_t find_hand(scene_t *scene, body_t *body, body_t *powerup) {
    vector_t centroid = body_get_centroid(body);
    vector_t hand;
    if (body_get_direction(body) == 0) {
        sprite_t *new_sprite = scene_get_sprite(scene, RIGHT_MAGNET_IDX);
        body_set_sprite(powerup, new_sprite);
        hand = (vector_t){.x = centroid.x + HAND_OFFSET.x, .y = centroid.y - HAND_OFFSET.y};
    }
    else {
        sprite_t *new_sprite = scene_get_sprite(scene, LEFT_MAGNET_IDX);
        body_set_sprite(powerup, new_sprite);
        hand = (vector_t){.x = centroid.x - HAND_OFFSET.x, .y = centroid.y - HAND_OFFSET.y};
    }
    return hand;
}