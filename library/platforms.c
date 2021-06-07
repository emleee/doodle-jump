#include "polygon.h"
#include "sdl_wrapper.h"
#include "platforms.h"
#include "constants.h"

const double PLATFORM_MASS = INFINITY;
const double TRICK_MASS = 0;

const vector_t NORMAL_DIMENSIONS = {.x = 146, .y = 35};
const vector_t SLIDING_DIMENSIONS = {.x = 146, .y = 35};
const vector_t DISAPPEARING_DIMENSIONS = {.x = 139, .y = 30};
const vector_t TRICK_DIMENSIONS = {.x = 1592/12, .y = 320/12};

const vector_t PLATFORM_VELOCITY = {.x = 200, .y = 0};

body_t *make_platform(vector_t center, rgb_color_t color, char *info, int width, int height, sprite_t *sprite) {
    list_t *shape = make_rectangle(VEC_ZERO, width, height);
    body_t *platform = body_init_with_sprite(shape, PLATFORM_MASS, color, info, free, sprite);
    body_set_centroid(platform, center);
    return platform;
}

body_t *normal_platform(vector_t center, char *info) {
    sprite_t *sprite = create_sprite("PNGs/Grass_Platform.png", NORMAL_DIMENSIONS.x, NORMAL_DIMENSIONS.y);
    body_t *platform = make_platform(center, SPRITE_COLOR, info, NORMAL_DIMENSIONS.x, NORMAL_DIMENSIONS.y, sprite);
    return platform;
}

body_t *sliding_platform(vector_t center, char *info) {
    sprite_t *sprite = create_sprite("PNGs/Flower_Platform.png", SLIDING_DIMENSIONS.x, SLIDING_DIMENSIONS.y);
    body_t *sliding_platform = make_platform(center, SPRITE_COLOR, info, SLIDING_DIMENSIONS.x, SLIDING_DIMENSIONS.y, sprite);
    body_set_velocity(sliding_platform, PLATFORM_VELOCITY);
    return sliding_platform;
}

void sliding_bounce(body_t *sliding_platform) {
    vector_t adjust = VEC_ZERO;
    list_t *shape = body_get_shape(sliding_platform);
    vector_t velocity = body_get_velocity(sliding_platform);
    
    vector_t *worst_pt = list_get(shape, 0);
    for (size_t i = 1; i < list_size(shape); i++) {
        vector_t *pt = list_get(shape, i);
        if ((velocity.x < 0 && pt->x < worst_pt->x) || (velocity.x > 0 && pt->x > worst_pt->x))
        {
            worst_pt = pt;
        }
    }

    // adjust the platform to correct for bounce
    if (worst_pt->x < 0) {
        adjust.x = -worst_pt->x;
        velocity.x *= -1;
    }
    else if (worst_pt->x > SCREEN_DIMENSIONS.x) {
        adjust.x = SCREEN_DIMENSIONS.x - worst_pt->x;
        velocity.x *= -1;
    }

    body_set_centroid(sliding_platform, vec_add(body_get_centroid(sliding_platform), adjust));
    body_set_velocity(sliding_platform, velocity);
    list_free(shape);
}

body_t *disappearing_platform(vector_t center, char *info) {
    sprite_t *sprite = create_sprite("PNGs/Cloud_Platform.png", DISAPPEARING_DIMENSIONS.x, DISAPPEARING_DIMENSIONS.y);
    body_t *disappearing_platform = make_platform(center, SPRITE_COLOR, info, DISAPPEARING_DIMENSIONS.x, DISAPPEARING_DIMENSIONS.y, sprite);
    return disappearing_platform;
}

body_t *trick_platform(vector_t center, char *info) {
    sprite_t *sprite = create_sprite("PNGs/Branch_Platform.png", TRICK_DIMENSIONS.x, TRICK_DIMENSIONS.y);
    body_t *trick_platform = make_platform(center, SPRITE_COLOR, info, TRICK_DIMENSIONS.x, TRICK_DIMENSIONS.y, sprite);
    body_set_mass(trick_platform, TRICK_MASS);
    return trick_platform;
}
