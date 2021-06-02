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
#include "star.h"
// Different Screens
#include "game.h"
#include "restart.h"
#include "start.h"
#include "preferences.h"
#include "shop.h"

const double WIDTH2 = 720.0;
const double HEIGHT2 = 960.0;

const double BUTTON_X_RADIUS = 125;
const double BUTTON_Y_RADIUS = 75;

const double PLAYER_X_VELOCITY = 600;


// const double G = -150.0;

void on_key(char key, key_event_type_t type, double held_time, void *scene) {
    body_t *player = scene_get_body((scene_t *)scene, 0);
    body_t *magnet = NULL;
    for (int i = 0; i < scene_bodies(scene); i++) {
        if (strcmp(body_get_info(scene_get_body(scene, i)), "magnet") == 0 && body_get_second_info(scene_get_body(scene, i)) != NULL && strcmp(body_get_second_info(scene_get_body(scene, i)), "equipped") == 0) {
            magnet = scene_get_body(scene, i);
        }
    }
    vector_t body_velocity = body_get_velocity(player);
    // double mass = body_get_mass(player);
    if (type == KEY_PRESSED) {
        switch (key) {
            case RIGHT_ARROW:
                if (strcmp(scene_get_info(scene), "game")==0) {
                    if (body_get_sprite(player) == scene_get_sprite(scene, 1)) {
                        change_direction(player, scene_get_sprite(scene, 0));
                    }
                    else if (body_get_sprite(player) == scene_get_sprite(scene, 3)) {
                        change_direction(player, scene_get_sprite(scene, 2));
                    }
                    body_set_rotation(player, 0);
                    body_velocity.x = PLAYER_X_VELOCITY;
                    body_set_velocity(player, body_velocity);
                    if (magnet != NULL) {
                        body_set_velocity(magnet, body_velocity);
                    }
                    break;
                }
            case LEFT_ARROW:
                if (strcmp(scene_get_info(scene), "game")==0) {
                    if (body_get_sprite(player) == scene_get_sprite(scene, 0)) {
                        change_direction(player, scene_get_sprite(scene, 1));
                    }
                    else if (body_get_sprite(player) == scene_get_sprite(scene, 2)) {
                        change_direction(player, scene_get_sprite(scene, 3));
                    }
                    body_set_rotation(player, M_PI);
                    body_velocity.x = -1 * PLAYER_X_VELOCITY;
                    body_set_velocity(player, body_velocity);
                    if (magnet != NULL) {
                        body_set_velocity(magnet, body_velocity);
                    }
                    break;
                }
        }
    }
    else {
        body_velocity.x = 0;
        body_set_velocity(player, body_velocity);
        if (magnet != NULL) {
            body_set_velocity(magnet, body_velocity);
        }
    }
}

void mouse_click(int key, int x, int y, void *scene) {
    switch(key) {
        case SDL_BUTTON_LEFT:
            if (strcmp(scene_get_info(scene), "game") == 0) {
                game_mouse_click(scene, x, y);
            }
            else if (strcmp(scene_get_info(scene), "start") == 0) {
                start_mouse_click(scene, x, y, BUTTON_X_RADIUS, BUTTON_Y_RADIUS);
            }
            else if (strcmp(scene_get_info(scene), "restart") == 0) {
                restart_mouse_click(scene, x, y, BUTTON_X_RADIUS, BUTTON_Y_RADIUS);
            }
            else if (strcmp(scene_get_info(scene), "settings") == 0) {
                settings_mouse_click(scene, x, y, BUTTON_X_RADIUS, BUTTON_Y_RADIUS);
            }
    }
}

int main() {
    vector_t start_min = {.x = 0, .y = 0};
    vector_t start_max = {.x = WIDTH2, .y = HEIGHT2};
    sdl_init(start_min, start_max);
    srand(time(0));

    int *timer = malloc(sizeof(int));
    *timer = 0;
    int *powerup_timer = malloc(sizeof(int));
    *powerup_timer = 0;
    int *star_timer = malloc(sizeof(int));
    *star_timer = 0;
    int *instructions_timer = malloc(sizeof(int));
    *instructions_timer = 0;
    int star_score = 0;

    int start_timer = 0;

    sdl_on_key(on_key);
    sdl_mouse(mouse_click);
    scene_t *scene = make_start_scene();
    vector_t *center = malloc(sizeof(vector_t));
    center->x = WIDTH2/2;
    center->y = HEIGHT2/2;
    rgb_color_t color = {.r = 0, .g = 0, .b = 0};

    vector_t *scoring = malloc(sizeof(vector_t));
    scoring->x = 80; // magic numbers
    scoring->y = 20;
    double curr = 0.0;

    char *score = malloc(100*sizeof(char));

    body_t *doodle;

    bool enemy_present = false;
    while (!sdl_is_done(scene)) {
        if (strcmp(scene_get_info(scene), "game") == 0 && strcmp(body_get_info(scene_get_body(scene, 0)), "doodle") != 0) {
            scene_set_next_info(scene, "restart");
        }
        if (strcmp(scene_get_info(scene), scene_get_next_info(scene)) != 0) {
            if (strcmp(scene_get_next_info(scene), "game") == 0) {
                center->x = WIDTH2/2;
                center->y = HEIGHT2/2;
                sdl_set_center(*center);
                star_score += scene_stars(scene);
                scene_free(scene);
                free(score);
                free(timer);
                free(powerup_timer);
                free(star_timer); // what happens if we quit before it finished intructions
                timer = malloc(sizeof(int));
                *timer = 0;
                powerup_timer = malloc(sizeof(int));
                *powerup_timer = 0;
                star_timer = malloc(sizeof(int));
                *star_timer = 0;
                score = malloc(100*sizeof(char));
                scene = make_game_scene();
                doodle = scene_get_body(scene, 0);
            }
            else if (strcmp(scene_get_next_info(scene), "start") == 0) {
                center->x = WIDTH2/2;
                center->y = HEIGHT2/2;
                sdl_set_center(*center);
                star_score += scene_stars(scene);
                scene_free(scene);
                scene = make_start_scene();
            }
            else if (strcmp(scene_get_next_info(scene), "restart") == 0) {
                center->x = WIDTH2/2;
                center->y = HEIGHT2/2;
                sdl_set_center(*center);
                star_score += scene_stars(scene);
                scene_free(scene);
                scene = make_restart_scene(score);
            }
            else if (strcmp(scene_get_next_info(scene), "shop") == 0) {
                center->x = WIDTH2/2;
                center->y = HEIGHT2/2;
                sdl_set_center(*center);
                scene_free(scene);
                scene = make_shop_scene();
            }
            else if (strcmp(scene_get_next_info(scene), "settings") == 0) {
                star_score += scene_stars(scene);
                scene_free(scene);
                scene = make_settings_scene();
            }
        }
        if (strcmp(scene_get_info(scene), "game") == 0) {
            game_main(scene, doodle, star_timer, powerup_timer, timer, instructions_timer, center, score);
        }
        else if (strcmp(scene_get_info(scene), "start") == 0) {
            start_timer++;
            start_main(scene, start_timer);
            sdl_render_scene(scene);

            // double dt = time_since_last_tick();
            // scene_tick(scene,dt);
            // sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "restart") == 0) {
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "shop") == 0) {
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "settings") == 0) {
            sdl_render_scene(scene);
        }
    }

    // save the number of stars collected
    FILE *star_file = fopen("stars.txt", "r+");
    if (star_file == NULL) {
        printf("NULL file.\n");
    }
    char *star_reading = malloc(10*sizeof(char));
    char **throwaway = malloc(sizeof(char *));
    *throwaway = malloc(10*sizeof(char));
    if (fgets(star_reading, 5, star_file) == NULL) {
        printf("Error.\n");
    }
    else {
        star_reading[6] = '\0';
        star_score += (int)strtod(star_reading, throwaway);
    }
    sprintf(star_reading, "%i", star_score);
    fseek(star_file, 0, SEEK_SET);
    fputs(star_reading, star_file);


    // only save score if it's a high score
    FILE *score_file = fopen("highscore.txt", "r+");
    if (score_file == NULL) {
        printf("NULL file.\n");
    }
    char *score_reading = malloc(100*sizeof(char));
    if (fgets(score_reading, 5, score_file) == NULL) {
        printf("Error.\n");
        score+=7;
        fseek(score_file, 0, SEEK_SET);
        fputs(score, score_file);
    }
    else {
        score_reading[6] = '\0';
        double highscore = strtod(score_reading, throwaway);
        score+=7;
        curr = strtod(score, throwaway);
        if (curr > highscore) {
            fseek(score_file, 0, SEEK_SET);
            fputs(score, score_file);
        }
    }

    free(star_reading);
    fclose(star_file);
    fclose(score_file);
    free(score_reading);
    free(throwaway);
    scene_free(scene);
    return 0;
}
