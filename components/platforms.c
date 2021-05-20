#include <stddef.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "polygon.h"
#include "color.h"
#include "list.h"
#include <time.h>
#include <stdio.h>
#include "forces.h"
#include "collision.h"

const double WINDOW_WIDTH = 720.0;

const double PLATFORM_WIDTH = 60;
const double PLATFORM_HEIGHT = 20;
const double PLATFORM_MASS = INFINITY;

const rgb_color_t NORMAL_COLOR = {.r = 0.0/255.0, .g = 204.0/255.0, .b = 0.0/255.0};
const rgb_color_t SLIDING_COLOR = {.r = 77.0/255.0, .g = 166.0/255.0, .b = 255.0/255.0};
const rgb_color_t DISAPPEARING_COLOR = {.r = 255.0/255.0, .g = 204.0/255.0, .b = 255.0/255.0};
const rgb_color_t TRICK_COLOR = {.r = 102.0/255.0, .g = 51.0/255.0, .b = 0.0/255.0}; 

const vector_t PLATFORM_VELOCITY = {.x = 200, .y = 0};

body_t *make_platform(vector_t center, rgb_color_t color, char *info) {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {0, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {PLATFORM_WIDTH, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {PLATFORM_WIDTH, PLATFORM_HEIGHT};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {0, PLATFORM_HEIGHT};
    list_add(shape, v);

    body_t *platform = body_init_with_info(shape, PLATFORM_MASS, color, info, free);
    body_set_centroid(platform, center);
    return platform;
}

body_t *normal_platform(vector_t center, char *info) {
    body_t *platform = make_platform(center, NORMAL_COLOR, info);
    return platform;
}

body_t *sliding_platform(vector_t center, char *info) {
    body_t *sliding_platform = make_platform(center, SLIDING_COLOR, info);
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
    else if (worst_pt->x > WINDOW_WIDTH) {
        adjust.x = WINDOW_WIDTH - worst_pt->x;
        velocity.x *= -1;
    }

    body_set_centroid(sliding_platform, vec_add(body_get_centroid(sliding_platform), adjust));
    body_set_velocity(sliding_platform, velocity);
}

body_t *disappearing_platform(vector_t center, char *info) {
    body_t *disappearing_platform = make_platform(center, DISAPPEARING_COLOR, info);
    return disappearing_platform;
}

body_t *trick_platform(vector_t center, char *info) {
    body_t *trick_platform = make_platform(center, DISAPPEARING_COLOR, info);
    return trick_platform;
}