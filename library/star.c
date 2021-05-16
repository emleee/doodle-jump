#include <stddef.h>
#include <math.h>
#include "polygon.h"
#include <assert.h>
#include <stdlib.h>

const vector_t INITIAL_VELOCITY = {.x=100.0, .y=-200.0};

const double ACCELERATION = -400;

typedef struct star {
    list_t *points;
    float r;
    float g;
    float b;
    vector_t *velocity;
    int elasticity;
} star_t;

star_t *star_init(size_t initial_size) {
    star_t *star = malloc(sizeof(star_t));
    star->points = list_init(initial_size, free);
    assert (star->points != NULL);
    star->r = rand() / (float) RAND_MAX;
    star->g = rand() / (float) RAND_MAX;
    star->b = rand() / (float) RAND_MAX;
    star->elasticity = rand() % 100;

    star->velocity = malloc(sizeof(vector_t));

    star->velocity->x = INITIAL_VELOCITY.x;
    star->velocity->y = INITIAL_VELOCITY.y;
    return star;
}

star_t *make_star (vector_t center, int num_points, double radius) {
    star_t *star = star_init(num_points*2);
    double phase_shift = M_PI / num_points;
    for (int i = 0; i < num_points; i++) {
        // add outer vertices of star
        vector_t *outer_pt = malloc(sizeof(vector_t));
        outer_pt->x = radius * cos(2 * M_PI * i / num_points + M_PI / 2);
        outer_pt->y = radius * sin(2 * M_PI * i / num_points + M_PI / 2);
        list_add(star->points, outer_pt);

        // add inner vertices of star
        vector_t *inner_pt = malloc(sizeof(vector_t));
        inner_pt->x = (radius / 2) * cos(2 * M_PI * i / num_points + (M_PI / 2) + phase_shift);
        inner_pt->y = (radius / 2) * sin(2 * M_PI * i / num_points + (M_PI / 2) + phase_shift);
        list_add(star->points, inner_pt);
    }
    polygon_translate(star->points, center);
    return star;
}

void change_color(star_t *star, float r, float g, float b) {
    star->r = fmod((r + .1), 1);
    star->g = fmod((g + .05), 1);
    star->b = fmod((b + .15), 1);
}

list_t *get_points(star_t *star) {
    return star->points;
}

float get_r(star_t *star) {
    return star->r;
}

float get_g(star_t *star) {
    return star->g;
}

float get_b(star_t *star) {
    return star->b;
}

vector_t *get_velocity(star_t *star) {
    return star->velocity;
}

int get_elasticity(star_t *star) {
    return star->elasticity;
}

vector_t *update_velocity(star_t *star, double dt) {
    star->velocity->y += ACCELERATION*dt;
    return star->velocity;
}

void star_free(star_t *star) {
    list_free(star->points);
    free(star->velocity);
    free(star);
}