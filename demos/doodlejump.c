#include <stddef.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "sdl_wrapper.h"
#include "polygon.h"
#include "color.h"
#include "list.h"
#include <time.h>
#include <stdio.h>
#include "forces.h"
#include "collision.h"

const double WIDTH = 1000;
const double HEIGHT = 500;

const vector_t PLAYER_VELOCITY = {.x = 600, .y = 0};
const rgb_color_t DOODLE_BODY_COLOR = {.r = 0/255, .g = 25/255, .b = 25/255}; //transparent?
const double DOODLE_MASS = 10;

const double G = -10;

const double X_START = 54.5;
const double Y_START = 465;

// modify based on theresa's stuff
body_t *make_block(vector_t center, rgb_color_t color, char *info) {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {0, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {30, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {30, 89};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {0, 89};
    list_add(shape, v);

    body_t *block = body_init_with_info(shape, 10, color, info, free);
    body_set_centroid(block, center);
    return block;
}


scene_t *make_scene() {
    scene_t *scene = scene_init();

    // doodle
    char *doodle_info = malloc(7*sizeof(char));
    doodle_info[0] = '\0';
    strcat(doodle_info, "doodle");
    
    vector_t start = {.x = WIDTH/2, .y = 0};
    body_t *doodle = make_block(start, DOODLE_BODY_COLOR, doodle_info);
    body_set_velocity(doodle, BOOST);
    scene_add_body(scene, doodle);
    create_downward_gravity(scene, G, doodle);

    
    return scene;
}

bool in_screen(vector_t center, body_t *body) {
    list_t *points = body_get_shape(body);
    for (int i = 0; i < list_size(points); i++) {
        if (((vector_t *)list_get(points, i))->y > center.y - HEIGHT/2) {
            return true;
        }
    }
    return false;
}

// void on_key(char key, key_event_type_t type, double held_time, void *scene) {
//     body_t *player = scene_get_body((scene_t *)scene, 0);
//     if (type == KEY_PRESSED) {
//         switch (key) {
//             case RIGHT_ARROW:
//                 if (body_get_centroid(player).x >= MAX.x-THRESHOLD) {
//                     body_set_velocity(player, VEC_ZERO);
//                 }
//                 else {
//                     body_set_velocity(player, PLAYER_VELOCITY);   
//                 }           
//                 break;
//             case LEFT_ARROW:
//                 if (body_get_centroid(player).x <= MIN.x+THRESHOLD) {
//                     body_set_velocity(player, VEC_ZERO);
//                 }
//                 else {
//                     body_set_velocity(player, vec_negate(PLAYER_VELOCITY));
//                 }
//                 break;
//         }
//     }
// }


int main() {
    vector_t start_min = {.x = 0, .y = 0};
    vector_t start_max = {.x = WIDTH, .y = HEIGHT};
    sdl_init(start_min, start_max);
    srand(time(0));
    // sdl_on_key(on_key);
    scene_t *scene = make_scene();
    vector_t center = {.x = WIDTH/2, HEIGHT/2};

    while (!sdl_is_done(scene)) {
        double dt = time_since_last_tick();
        body_t *doodle = scene_get_body(scene, 0);

        if (body_get_info(doodle) != "doodle") {
            // PLAYER LOSES, REPLACE BREAK WITH ACTUAL CODE
            break;
        }

        // freeing platforms, etc that are out of the screen
        for(int i = 1; i < scene_bodies(scene); i++) {
            if (!in_screen(center, scene_get_body(scene, i))) {
                scene_remove_body(scene, i);
            }
        }

        if (body_get_centroid(doodle).y > center.y) {
            center.y = body_get_centroid(doodle).y;
            sdl_set_center(center);
        }


        scene_tick(scene, dt);
        sdl_render_scene(scene);
    }
    scene_free(scene);
}
