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

const double PLATFORM_WIDTH = 60;
const double PLATFORM_HEIGHT = 20;
const double PLATFORM_MASS = INFINITY;

body_t *make_block(vector_t center, rgb_color_t color, char *info) {
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

    body_t *block = body_init_with_info(shape, BLOCK_MASS, color, info, free);
    body_set_centroid(block, center);
    return block;
}