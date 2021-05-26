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
#include "platforms.h"
#include "sdl_wrapper.h"
#include "game_sprites.h"
#include "SDL2/SDL_mixer.h"
#include "text.h"
#include "test_util.h"
#include "powerups.h"

const rgb_color_t BOOST_COLOR ={.r = 106.0/255, .g = 77.0/255, .b = 255.0/255};
const double RADIUS = 152.0;

body_t *make_boost(scene_t *scene, vector_t center){
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {0, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {20, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {20, 20};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {0, 20};
    list_add(shape, v);
    char *info = malloc(sizeof(char)*6);
    strcpy(info, "boost");
    body_t *boost = body_init_with_info(shape, INFINITY, BOOST_COLOR, info, free);
    body_set_centroid(boost, center);

    scene_add_body(scene, boost);
    create_boost_collision(scene, 0, scene_get_body(scene, 0), boost);
    return boost;
}

body_t *make_immunity(scene_t *scene, vector_t center){
    list_t *shape = list_init(20, free); // 13/16 + 1
    for (int i = 0; i < 20; i++) {
        vector_t *pt = malloc(sizeof(vector_t));
        pt->x = RADIUS * cos(2 * M_PI * i / 20 + M_PI / 2);
        pt->y = RADIUS * sin(2 * M_PI * i / 20 + M_PI / 2);
        list_add(shape, pt);
    }
    char *info = malloc(sizeof(char)*9);
    strcpy(info, "immunity");
    body_t *boost = body_init_with_info(shape, INFINITY, BOOST_COLOR, info, free);
    body_set_centroid(boost, center);
    scene_add_body(scene, boost);
    return boost;
}