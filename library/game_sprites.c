#include "polygon.h"
#include "sdl_wrapper.h"
#include "game_sprites.h"
#include "constants.h"

// direction change offsets
const vector_t FACE_RIGHT = {.x = -21, .y = 0};
const vector_t FACE_LEFT = {.x = 21, .y = 0};

// magnet stuff
const int RIGHT_MAGNET_IDX = 4;
const int LEFT_MAGNET_IDX = 5;

// position stuff
const vector_t HAND_OFFSET = {.x = 70, .y = 15};
const vector_t MOUTH_OFFSET = {.x = 13, .y = 24};

// background stuff
const vector_t BACKGROUND_CORNER = {.x = -1, .y = -1};
const double BACKGROUND_BODY_LENGTH = 2;

body_t *make_doodle(vector_t center, char *info, sprite_t *sprite) {
    list_t *shape = make_rectangle(VEC_ZERO, DOODLE_WIDTH, DOODLE_HEIGHT);

    char *second_info = malloc(5 * sizeof(char));
    strcpy(second_info, "jump");

    body_t *doodle = body_init_with_sprite(shape, DOODLE_MASS, SPRITE_COLOR, info, free, sprite);
    body_set_centroid(doodle, center);
    body_set_second_info(doodle, second_info);

    return doodle;
}

body_t *make_background_body(char *file, vector_t center) {
    list_t *shape = make_rectangle(BACKGROUND_CORNER, BACKGROUND_BODY_LENGTH, BACKGROUND_BODY_LENGTH);
    
    char *info = malloc(11 * sizeof(char));
    strcpy(info, "background");

    sprite_t *sprite = create_sprite(file, SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y);
    body_t *background = body_init_with_sprite(shape, 1, SPRITE_COLOR, info, free, sprite);
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
        hand = (vector_t){.x = centroid.x + HAND_OFFSET.x, .y = centroid.y - HAND_OFFSET.y};
    }
    else {
        hand = (vector_t){.x = centroid.x - HAND_OFFSET.x, .y = centroid.y - HAND_OFFSET.y};
    }
    return hand;
}