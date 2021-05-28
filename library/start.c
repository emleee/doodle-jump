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
#include "preferences.h"
#include "powerups.h"
#include "star.h"
#include "start.h"

const double START_HEIGHT = 960.0;
const double START_DOODLE_HEIGHT = 148.0;
const vector_t VELOCITY = {.x = 0, .y = 300};

const rgb_color_t START_DOODLE_BODY_COLOR = {.r = 176.0/255, .g = 128.0/255, .b = 124.0/255};
const double START_MAX_JUMP = 295.0;

const double START_G = -150.0;

scene_t *make_start_scene() {
    char *scene_info = malloc(6*sizeof(char));
    strcpy(scene_info, "start");
    scene_t *scene = scene_init_with_info(scene_info, free);

    rgb_color_t color = {.r = 0, .g = 0, .b = 0};
    vector_t *point2 = malloc(sizeof(vector_t));
    point2->x = 250; // remove magic numbers
    point2->y = 200;
    text_t *text2 = text_create("Start", color, 22, point2);
    scene_add_text(scene, text2);

    vector_t *point3 = malloc(sizeof(vector_t));
    point3->x = 250; // remove magic numbers
    point3->y = 300;
    text_t *text3 = text_create("Settings", color, 22, point3);
    scene_add_text(scene, text3);

    // body_t *start_button = make_button(*point2);
    // scene_add_body(scene, start_button);

    vector_t *point = malloc(sizeof(vector_t));
    point->x = 250; // remove magic numbers
    point->y = 50;
    text_t *text = text_create("Doodle Jump: Fairy Tail", color, 28, point);
    scene_add_text(scene, text);
    char *doodle_info = malloc(7*sizeof(char));
    strcpy(doodle_info, "doodle");
    vector_t start = {.x = 50, .y = VELOCITY.y/2 + START_DOODLE_HEIGHT/2};
    body_t *doodle = make_doodle(start, START_DOODLE_BODY_COLOR, doodle_info);

    sprite_t *right_jump = body_get_sprite(doodle);
    scene_add_sprite(scene, right_jump);

    body_set_velocity(doodle, VELOCITY);
    scene_add_body(scene, doodle);
    create_downward_gravity(scene, START_G, doodle);

    char *other_info = malloc(22*sizeof(char));
    strcpy(other_info, "nonessential platform");
    vector_t safety_platform_center = {.x = 50, .y = START_MAX_JUMP/2};
    body_t *safety_platform = normal_platform(safety_platform_center, other_info);
    scene_add_body(scene, safety_platform);
    create_platform_collision(scene, 0, doodle, safety_platform);

    body_t *background1 = make_background_body((vector_t){.x = 0, .y = START_HEIGHT});
    body_t *background2 = make_background_body((vector_t){.x = 0, .y = 2*START_HEIGHT});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);


    return scene;
}