#include "platforms.h"
#include "sdl_wrapper.h"

const double WINDOW_LENGTH = 720.0;

const double PLATFORM_WIDTH = 146;
const double PLATFORM_HEIGHT = 35;
const double PLATFORM_MASS = INFINITY;

const rgb_color_t NORMAL_COLOR = {.r = 0.0/255.0, .g = 204.0/255.0, .b = 0.0/255.0};
const rgb_color_t SLIDING_COLOR = {.r = 77.0/255.0, .g = 166.0/255.0, .b = 255.0/255.0};
const rgb_color_t DISAPPEARING_COLOR = {.r = 255.0/255.0, .g = 204.0/255.0, .b = 255.0/255.0};
const rgb_color_t TRICK_COLOR = {.r = 102.0/255.0, .g = 51.0/255.0, .b = 0.0/255.0};

const vector_t PLATFORM_VELOCITY = {.x = 200, .y = 0};

body_t *make_platform(vector_t center, rgb_color_t color, char *info, int width, int height, sprite_t *sprite) {
    list_t *shape = make_rectangle(VEC_ZERO, width, height);
    body_t *platform = body_init_with_info(shape, PLATFORM_MASS, color, info, free);
    body_set_sprite(platform, sprite);
    body_set_centroid(platform, center);
    return platform;
}

body_t *normal_platform(vector_t center, char *info) {
    sprite_t *sprite = create_sprite("PNGs/Grass_Platform.png", 146, 35);
    body_t *platform = make_platform(center, NORMAL_COLOR, info, PLATFORM_WIDTH, PLATFORM_HEIGHT, sprite);
    return platform;
}

body_t *sliding_platform(vector_t center, char *info) {
    sprite_t *sprite = create_sprite("PNGs/Flower_Platform.png", 146, 35);
    body_t *sliding_platform = make_platform(center, SLIDING_COLOR, info, PLATFORM_WIDTH, PLATFORM_HEIGHT, sprite);
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
    else if (worst_pt->x > WINDOW_LENGTH) {
        adjust.x = WINDOW_LENGTH - worst_pt->x;
        velocity.x *= -1;
    }

    body_set_centroid(sliding_platform, vec_add(body_get_centroid(sliding_platform), adjust));
    body_set_velocity(sliding_platform, velocity);
}

body_t *disappearing_platform(vector_t center, char *info) {
    sprite_t *sprite = create_sprite("PNGs/Cloud_Platform.png", 139, 30);
    body_t *disappearing_platform = make_platform(center, DISAPPEARING_COLOR, info, 139, 30, sprite);
    return disappearing_platform;
}

body_t *trick_platform(vector_t center, char *info) {
    sprite_t *sprite = create_sprite("PNGs/Branch_Platform.png", 1592/12, 320/12);
    body_t *trick_platform = make_platform(center, DISAPPEARING_COLOR, info, 1592/12, 320/12, sprite);
    body_set_mass(trick_platform, 0);
    return trick_platform;
}
