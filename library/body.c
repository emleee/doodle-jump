#include "body.h"
#include "polygon.h"
#include "list.h"
#include "vector.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <sprite.h>
#include "game_sprites.h"
#include "sdl_wrapper.h"

typedef struct body {
    list_t *shape;
    double mass;
    rgb_color_t color;
    vector_t velocity;
    vector_t centroid;
    double direction;
    vector_t force;
    vector_t impulse;
    void *info;
    void *info2;
    free_func_t info_freer;
    bool remove;
    sprite_t *sprite;
} body_t;

body_t *body_init(list_t *shape, double mass, rgb_color_t color) {
    body_t *body = malloc(sizeof(body_t));
    assert(body != NULL);
    body->shape = list_init(list_size(shape), get_freer(shape));
    body->shape = shape;
    body->mass = mass;
    body->color = color;
    body->velocity = VEC_ZERO;
    body->centroid = polygon_centroid(shape);
    body->direction = 0.0;
    body->force = VEC_ZERO;
    body->impulse = VEC_ZERO;
    body->info = NULL;
    body->info2 = NULL;
    body->info_freer = NULL;
    body->remove = false;
    body->sprite = NULL;
    return body;
}

body_t *body_init_with_info(list_t *shape, double mass, rgb_color_t color, void *info, free_func_t info_freer) {
    body_t *body = body_init(shape, mass, color);
    body->info = info;
    body->info_freer = info_freer;
    return body;
}

body_t *body_init_with_sprite(list_t *shape, double mass, rgb_color_t color, void *info, free_func_t info_freer, sprite_t *sprite) {
    body_t *body = body_init_with_info(shape, mass, color, info, info_freer);
    body->sprite = sprite;
    return body;
}

sprite_t *body_get_sprite(body_t *body) {
    return body->sprite;
}

void body_set_sprite(body_t *body, sprite_t *sprite) {
    body->sprite = sprite;
    // sprite_set_center(body->sprite, body_get_centroid(body));
}

void body_free(body_t *body) {
    list_free(body->shape);
    if (body->sprite != NULL) {
        sprite_free(body->sprite);
    }
    body->info_freer(body->info);
    body->info_freer(body->info2);
    free(body);
}

list_t *body_get_shape(body_t *body) {
    list_t *shape_copy = list_init(list_size(body->shape), get_freer(body->shape));
    for (int i = 0; i < list_size(body->shape); i++) {
        vector_t *vec = malloc(sizeof(vector_t));
        vector_t *old = list_get(body->shape, i);
        vec->x = old->x;
        vec->y = old->y;
        list_add(shape_copy, vec);
    }
    return shape_copy;
}

vector_t body_get_centroid(body_t *body) {
    return body->centroid;
}

vector_t body_get_velocity(body_t *body) {
    return body->velocity;
}

double body_get_mass(body_t *body) {
    return body->mass;
}

rgb_color_t body_get_color(body_t *body) {
    return body->color;
}

double body_get_direction(body_t *body) {
    return body->direction;
}

void body_set_centroid(body_t *body, vector_t x) {
    polygon_translate(body->shape, vec_subtract(x, body->centroid));
    if (body->sprite != NULL) {
        if (body->info2 != NULL && strcmp(body->info2, "jump") == 0) {
            if (body->direction == 0) {
                vector_t sprite_pos = vec_add(get_window_position(x, get_window_center()), RIGHT_OFFSET);
                sprite_set_center(body->sprite, sprite_pos);
            }
            else {
                vector_t sprite_pos = vec_add(get_window_position(x, get_window_center()), LEFT_OFFSET);
                sprite_set_center(body->sprite, sprite_pos);
            }
        }
        else if (body->info2 != NULL && strcmp(body->info2, "winged") == 0) {
            if (body->direction == 0) {
                vector_t sprite_pos = vec_add(get_window_position(x, get_window_center()), LEFT_OFFSET);
                sprite_set_center(body->sprite, sprite_pos);
            }
            else {
                vector_t sprite_pos = vec_add(get_window_position(x, get_window_center()), RIGHT_OFFSET);
                sprite_set_center(body->sprite, sprite_pos);
            }
        }
        else if (strcmp(body->info, "background") == 0) {
            vector_t sprite_pos = vec_add(get_window_position(x, get_window_center()), (vector_t){.x = 360, .y = 480});
            sprite_set_center(body->sprite, sprite_pos);
        }
        else {
            sprite_set_center(body->sprite, get_window_position(x, get_window_center()));
        }
    }
    body->centroid = x;
}

void body_set_velocity(body_t *body, vector_t v) {
    body->velocity = v;
}

void body_set_direction(body_t *body, double direction) {
    body->direction = direction;
}

void body_set_rotation(body_t *body, double angle) {
    polygon_rotate(body->shape, angle - body->direction, body->centroid);
    body_set_direction(body, angle);
}

void body_set_mass(body_t *body, double mass) {
    body->mass = mass;
}

void body_set_second_info(body_t *body, void *info) {
    body->info_freer(body->info2);
    body->info2 = info;
}

void body_add_force(body_t *body, vector_t force) {
    body->force = vec_add(body->force, vec_multiply(1/body->mass, force));
}

void body_add_impulse(body_t *body, vector_t impulse) {
    body->impulse = vec_add(body->impulse, vec_multiply(1/body->mass, impulse));
}

void body_tick(body_t *body, double dt) {
    vector_t new_velocity = vec_add(body->velocity, vec_add(body->impulse, vec_multiply(dt, body->force)));
    vector_t average = vec_multiply(0.5, vec_add(new_velocity, body->velocity));
    body_set_centroid(body, vec_add(body->centroid, vec_multiply(dt, average)));
    body->velocity = new_velocity;
    body->force = VEC_ZERO;
    body->impulse = VEC_ZERO;
}

void *body_get_info(body_t *body) {
    return body->info;
}

void *body_get_second_info(body_t *body) {
    return body->info2;
}

void body_remove(body_t *body) {
    body->remove = true;
}

bool body_is_removed(body_t *body) {
    return body->remove;
}