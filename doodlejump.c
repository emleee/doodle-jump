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

const vector_t MAX = {.x=1000.0, .y=500.0};
const vector_t MIN = {.x=0.0, .y=0.0};

const double X_START = 54.5;
const double Y_START = 465;

scene_t *make_scene() {
    scene_t *scene = scene_init();

    // doodle
    char *doodle_info = malloc(7*sizeof(char));
    doodle_info[0] = '\0';
    strcat(player_info, "doodle");
    
    vector_t start = {.x = MAX.x/2, .y = PLAYER_HEIGHT};
    body_t *doodle = make_block(start, COLOR, player_info);
    scene_add_body(scene, doodle);
    
    
    return scene;
}

bool in_screen(body_t *body) {
    list_t *points = body_get_shape(body);
    for (int i = 0; i < list_size(points); i++) {
        if (((vector_t *)list_get(points, i))->y > MIN.y) {
            return true;
        }
    }
    return false;
}

void on_key(char key, key_event_type_t type, double held_time, void *scene) {
    body_t *player = scene_get_body((scene_t *)scene, 0);
    if (type == KEY_PRESSED) {
        switch (key) {
            case RIGHT_ARROW:
                if (body_get_centroid(player).x >= MAX.x-THRESHOLD) {
                    body_set_velocity(player, VEC_ZERO);
                }
                else {
                    body_set_velocity(player, PLAYER_VELOCITY);   
                }           
                break;
            case LEFT_ARROW:
                if (body_get_centroid(player).x <= MIN.x+THRESHOLD) {
                    body_set_velocity(player, VEC_ZERO);
                }
                else {
                    body_set_velocity(player, vec_negate(PLAYER_VELOCITY));
                }
                break;
        }
    }
    else {
        body_set_velocity(player, VEC_ZERO);
    }
}


int main() {
    sdl_init(MIN, MAX);
    srand(time(0));
    sdl_on_key(on_key);
    scene_t *scene = make_scene();

    while (!sdl_is_done(scene)) {
        double dt = time_since_last_tick();

        if (body_get_info(scene_get_body(scene, 0)) != "doodle") {
            // PLAYER LOSES, REPLACE BREAK WITH ACTUAL CODE
            break;
        }
        for(int i = 1; i < scene_bodies(scene); i++) {
            if (!in_screen(scene_get_body(scene, i))) {
                scene_remove_body(scene, i);
            }
        }


        scene_tick(scene, dt);
        sdl_render_scene(scene);
    }
    scene_free(scene);
}
