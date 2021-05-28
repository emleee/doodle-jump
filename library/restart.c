#include <stddef.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include <stdio.h>
#include "sdl_wrapper.h"
#include "text.h"
#include "test_util.h"
#include "restart.h"

const double RESTART_HEIGHT = 960.0;
const double VELOCITY_X = 600;

scene_t *make_restart_scene(char *score) { // add something to keep track score vs high score and the falling/sad doodle
    char *scene_info = malloc(8*sizeof(char));
    strcpy(scene_info, "restart");
    scene_t *scene = scene_init_with_info(scene_info, free);

    rgb_color_t color = {.r = 0, .g = 0, .b = 0};
    vector_t *point1 = malloc(sizeof(vector_t));
    point1->x = 250; // remove magic numbers
    point1->y = 200;
    text_t *text1 = text_create(score, color, 22, point1);
    scene_add_text(scene, text1);

    vector_t *point = malloc(sizeof(vector_t));
    point->x = 250; // remove magic numbers
    point->y = 400;
    text_t *text = text_create("Restart", color, 22, point);
    scene_add_text(scene, text);

    vector_t *point2 = malloc(sizeof(vector_t));
    point2->x = 250; // remove magic numbers
    point2->y = 500;
    text_t *text2 = text_create("Home", color, 22, point2);
    scene_add_text(scene, text2);

    body_t *background1 = make_background_body((vector_t){.x = 0, .y = RESTART_HEIGHT});
    body_t *background2 = make_background_body((vector_t){.x = 0, .y = 2*RESTART_HEIGHT});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);
    return scene;
}