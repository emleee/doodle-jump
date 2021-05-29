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

const rgb_color_t BOOST_COLOR = {.r = 106.0/255, .g = 77.0/255, .b = 255.0/255};
const rgb_color_t IMMUNITY_COLOR = {.r = 0.54, .g = 0.54, .b = 0.54};
const rgb_color_t MAGNET_COLOR = {.r = 1, .g = 1, .b = 0.2};
const double RADIUS = 20.0; //152.0

const int BOOST_IDX = 1;
const int IMMUNITY_IDX = 2;
const int MAGNET_IDX = 3;

const int NEWTONIAN_GRAVITY = -150;
const int MAGNET_GRAVITY = 100;

void make_powerup(scene_t *scene, vector_t center) {
    int num_powerups = 0;
    for (int i = 0; i < scene_bodies(scene); i++) {
        body_t *body = scene_get_body(scene, i);
        char *info = body_get_info(body);
        if (strcmp(info, "boost") == 0 || strcmp(info, "immunity") == 0 || strcmp(info, "magnet") == 0) {
            num_powerups++;
        }
    }
    int idx = (rand() % (3 - 1 + 1)) + 1;
    if (num_powerups == 0) {
        if (idx == BOOST_IDX) {
            make_boost(scene, center);
        }
        else if (idx == IMMUNITY_IDX) {
            make_immunity(scene, center);
        }
        else if (idx == MAGNET_IDX) {
            make_magnet(scene, center);
        }
    }
    printf("x: %f, y: %f\n", center.x, center.y);
}

    int random = 0;
    char *info = body_get_info(scene_get_body(scene, random));
    bool conflict = false;
    int counter = 0;
    do {
        if (counter == scene_bodies(scene)) {
            // too many stars for now, just return
            return;
        }

        while (strcmp("normal platform", info) != 0) {
            random = rand() % scene_bodies(scene);
            info = body_get_info(scene_get_body(scene, random));
        }

        // check if platform already has a star on it
        body_t *body2 = scene_get_body(scene, random);
        vector_t centroid = body_get_centroid(body2);
        centroid.y += 40;

        for (size_t j = 0; j < scene_bodies(scene); j++) {
            body_t *body1 = scene_get_body(scene, j);
            if (strcmp(body_get_info(body1), "star") == 0 && random != j && vec_is_close(body_get_centroid(body1), centroid)) {
                conflict = true;
            }
        }
        counter++;
    }
    while (conflict);

    body_t *platform = scene_get_body(scene, random);
    vector_t center = body_get_centroid(platform);
    center.y += 40; // magic number for offset

    star_t *starframe = make_star(center, 5, 17); // magic number for num points, radius
    rgb_color_t color = {.r = 1, .g = 1, .b = 0}; // make const for 'yellow' star color
    char *star_info = malloc(5*sizeof(char));
    strcpy(star_info, "star");
    body_t *star = body_init_with_info(get_points(starframe), INFINITY, color, star_info, free);

    create_star_collision(scene, 0, scene_get_body(scene, 0), star);
    scene_add_body(scene, star);
}




body_t *make_boost(scene_t *scene, vector_t center){
    printf("made boost\n");
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

body_t *make_immunity(scene_t *scene, vector_t center) {
    printf("made immunity\n");
    list_t *shape = list_init(20, free); // 13/16 + 1
    for (int i = 0; i < 20; i++) {
        vector_t *pt = malloc(sizeof(vector_t));
        pt->x = RADIUS * cos(2 * M_PI * i / 20 + M_PI / 2);
        pt->y = RADIUS * sin(2 * M_PI * i / 20 + M_PI / 2);
        list_add(shape, pt);
    }
    char *info = malloc(sizeof(char)*9);
    strcpy(info, "immunity");
    body_t *immunity = body_init_with_info(shape, INFINITY, IMMUNITY_COLOR, info, free);
    body_set_centroid(immunity, center);
    scene_add_body(scene, immunity);
    body_t *doodle = scene_get_body(scene, 0);
    create_powerup_collision(scene, 0, doodle, immunity);
    return immunity;
}

body_t *make_magnet(scene_t *scene, vector_t center) {
    printf("made magnet\n");
    body_t *doodle = scene_get_body(scene, 0);
    list_t *shape = list_init(20, free);
    for (int i = 0; i < 20; i++) {
        vector_t *pt = malloc(sizeof(vector_t));
        pt->x = RADIUS * cos(2 * M_PI * i / 20 + M_PI / 2);
        pt->y = RADIUS * sin(2 * M_PI * i / 20 + M_PI / 2);
        list_add(shape, pt);
    }
    char *info = malloc(sizeof(char)*7);
    strcpy(info, "magnet");
    body_t *magnet = body_init_with_info(shape, INFINITY, MAGNET_COLOR, info, free);
    body_set_centroid(magnet, center);
    scene_add_body(scene, magnet);
    return magnet;
}



void magnet_powerup(scene_t *scene) {
    body_t *doodle = scene_get_body(scene, 0);
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        body_t *body = scene_get_body(scene, i);
        if (strcmp(body_get_info(body), "star") == 0) {
            create_magnet(scene, MAGNET_GRAVITY, magnet, body);
        }
        if (strcmp(body_get_info(body), "normal") == 0 || strcmp(body_get_info(body), "normal platform") == 0 || strcmp(body_get_info(body), "essential") == 0 || strcmp(body_get_info(body), "essential platform") == 0) {
            create_platform_collision(scene, 0, magnet, body);
        }
    }
    create_powerup_collision(scene, 0, doodle, magnet);
    create_downward_gravity(scene, NEWTONIAN_GRAVITY, magnet);
}