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

body_t *make_boost(vector_t center){
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
    body_t *boost = body_init_with_info(shape, 10, BOOST_COLOR, info, free);
    body_set_centroid(boost, center);
    return boost;
}
