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

const double WIDTH2 = 720.0;
const double HEIGHT2 = 960.0;

const double BUTTON_X_RADIUS = 125;
const double BUTTON_Y_RADIUS = 75;

// const double MAX_PLATFORMS = 12;
// const double PLATFORM_WIDTH2 = 146;
// const double PLATFORM_HEIGHT2 = 35;

const vector_t START_VELOCITY = {.x = 0, .y = 300};
const double PLAYER_X_VELOCITY = 600;

const rgb_color_t DOODLE_BODY_COLOR = {.r = 176.0/255, .g = 128.0/255, .b = 124.0/255};
const double MAX_JUMP = 295.0;

const double G = -150.0;

void on_key(char key, key_event_type_t type, double held_time, void *scene) {
    body_t *player = scene_get_body((scene_t *)scene, 0);
    vector_t body_velocity = body_get_velocity(player);
    // double mass = body_get_mass(player);
    if (type == KEY_PRESSED) {
        switch (key) {
            case RIGHT_ARROW:
                if (strcmp(scene_get_info(scene), "game")==0) {
                    if (body_get_sprite(player) == scene_get_sprite(scene, 1)) {
                        change_direction(player, scene_get_sprite(scene, 0));
                    }
                    body_set_rotation(player, 0);
                    body_velocity.x = PLAYER_X_VELOCITY;
                    body_set_velocity(player, body_velocity);
                    break;
                }
            case LEFT_ARROW:
                if (strcmp(scene_get_info(scene), "game")==0) {
                    if (body_get_sprite(player) == scene_get_sprite(scene, 0)) {
                        change_direction(player, scene_get_sprite(scene, 1));
                    }
                    body_set_rotation(player, M_PI);
                    body_velocity.x = -1 * PLAYER_X_VELOCITY;
                    body_set_velocity(player, body_velocity);
                    break;
                }
        }
    }
    else {
        body_velocity.x = 0;
        body_set_velocity(player, body_velocity);
    }
}

void mouse_click(int key, int x, int y, void *scene) {
    switch(key) {
        case SDL_BUTTON_LEFT:
            if (strcmp(scene_get_info(scene), "game") == 0) {
                game_mouse_click(scene, x, y);
            }
            else if (strcmp(scene_get_info(scene), "start") == 0) {
                if (x < (250 + BUTTON_X_RADIUS) && x > (250 - BUTTON_X_RADIUS)) {
                    if (y < (200 + BUTTON_Y_RADIUS) && y > (200 - BUTTON_Y_RADIUS)) {
                        char *game_info = malloc(5*sizeof(char));
                        strcpy(game_info, "game");
                        scene_set_next_info(scene, game_info);
                    }
                    if (y < (300 + BUTTON_Y_RADIUS) && y > (300 - BUTTON_Y_RADIUS)) {
                        char *settings_info = malloc(9*sizeof(char));
                        strcpy(settings_info, "settings");
                        scene_set_next_info(scene, settings_info);
                    }
                }
            }
            else if (strcmp(scene_get_info(scene), "restart") == 0) {
                if (x < (250 + BUTTON_X_RADIUS) && x > (250 - BUTTON_X_RADIUS)) {
                    if (y < (400 + BUTTON_Y_RADIUS) && y > (400 - BUTTON_Y_RADIUS)) {
                        char *game_info = malloc(5*sizeof(char));
                        strcpy(game_info, "game");
                        scene_set_next_info(scene, game_info);
                    }
                    else if (y < (500 + BUTTON_Y_RADIUS) && y > (500 - BUTTON_Y_RADIUS)) {
                        char *start_info = malloc(6*sizeof(char));
                        strcpy(start_info, "start");
                        scene_set_next_info(scene, start_info);
                    }
                }
            }
            else if (strcmp(scene_get_info(scene), "settings") == 0) {
                settings_mouse_click(scene, x, y);
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
    int star_score = 0;

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
    // char *buffer = malloc(100*sizeof(char));

    body_t *doodle;

    bool enemy_present = false;
    while (!sdl_is_done(scene)) {
        if (strcmp(scene_get_info(scene), scene_get_next_info(scene)) != 0) {
            if (strcmp(scene_get_next_info(scene), "game") == 0) {
                center->x = WIDTH2/2;
                center->y = HEIGHT2/2;
                sdl_set_center(*center);
                scene_free(scene);
                free(score);
                score = malloc(100*sizeof(char));
                scene = make_game_scene();
                doodle = scene_get_body(scene, 0);
            }
            else if (strcmp(scene_get_next_info(scene), "start") == 0) {
                center->x = WIDTH2/2;
                center->y = HEIGHT2/2;
                sdl_set_center(*center);
                scene_free(scene);
                scene = make_start_scene();
            }
            else if (strcmp(scene_get_next_info(scene), "restart") == 0) {
                center->x = WIDTH2/2;
                center->y = HEIGHT2/2;
                sdl_set_center(*center);
                star_score = scene_stars(scene);
                scene_free(scene);
                scene = make_restart_scene(score);
            }
            else if (strcmp(scene_get_next_info(scene), "settings") == 0) {
                scene_free(scene);
                scene = make_settings_scene();
            }
        }
        if (strcmp(scene_get_info(scene), "game") == 0) {
           game_main(scene, doodle, star_timer, powerup_timer, timer, center, score);
        }
        else if (strcmp(scene_get_info(scene), "start") == 0) {
            double dt = time_since_last_tick();
            scene_tick(scene,dt);
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "restart") == 0) {
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "settings") == 0) {
            sdl_render_scene(scene);
        }
    }

    // save the number of stars collected
    printf("\n%i\n", star_score);

    // only save score if it's a high score
    FILE *file = fopen("highscore.txt", "w+");
    if (file == NULL) {
        printf("NULL file.\n");
    }
    char *buffer2 = malloc(100*sizeof(char));
    fgets(buffer2, 5, file);
    char **throwaway = malloc(sizeof(char *));
    double highscore = strtod(buffer2, throwaway);
    curr = strtod(score+=7, throwaway);
    if (curr > highscore) {
        // score+=7;
        fputs(score, file);
    }


   /**
     * star saving
     *
     * create a list? then add any star collected to the list
     * or just have a variable and every time a star is collected increment the var
     * save ending in a file?
     *
     * star spawning
     *
     * pick a random platform, get its center or smth and move vector y up a bit
     * draw a star there and add it to scene, add magnetic force
     * how to make it disappear? destructive collision?
     **/



    fclose(file);
    // free(buffer);
    free(buffer2);
    free(throwaway);
    scene_free(scene);
    return 0;
}
