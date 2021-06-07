// Different Screens
#include <stdlib.h>
#include "game.h"
#include "restart.h"
#include "start.h"
#include "shop.h"

#include "constants.h"

const double BUTTON_X_RADIUS = 125;
const double BUTTON_Y_RADIUS = 75;

const double PLAYER_X_VELOCITY = 600;

const int SHOP_TIMER = 750;

const vector_t SCOREBOX_CENTER = {.x = 80, .y = 20};

void on_key(char key, key_event_type_t type, double held_time, void *scene) {
    body_t *player = scene_get_body((scene_t *)scene, 0);
    body_t *magnet = NULL;
    for (int i = 0; i < scene_bodies(scene); i++) {
        if (strcmp(body_get_info(scene_get_body(scene, i)), "magnet") == 0 && body_get_second_info(scene_get_body(scene, i)) != NULL && strcmp(body_get_second_info(scene_get_body(scene, i)), "equipped") == 0) {
            magnet = scene_get_body(scene, i);
        }
    }
    vector_t body_velocity = body_get_velocity(player);
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
            else if (strcmp(scene_get_info(scene), "shop") == 0) {
                shop_mouse_click(scene, x, y);
            }
            else if (strcmp(scene_get_info(scene), "instructions") == 0) {
                instructions_mouse_click(scene, x, y, BUTTON_X_RADIUS, BUTTON_Y_RADIUS);
            }
    }
}

int main() {
    vector_t start_min = {.x = 0, .y = 0};
    vector_t start_max = {.x = SCREEN_DIMENSIONS.x, .y = SCREEN_DIMENSIONS.y};
    sdl_init(start_min, start_max);
    srand(time(0));
    
    int *powerup_timer = malloc(sizeof(int));
    *powerup_timer = 0;
    int *star_timer = malloc(sizeof(int));
    *star_timer = 0;
    int star_score = 0;

    int start_timer = 0;
    int shop_timer = 0;

    sdl_on_key(on_key);
    sdl_mouse(mouse_click);
    scene_t *scene = make_start_scene();
    vector_t *center = malloc(sizeof(vector_t));
    center->x = SCREEN_DIMENSIONS.x/2;
    center->y = SCREEN_DIMENSIONS.y/2;

    vector_t *scoring = malloc(sizeof(vector_t));
    scoring->x = SCOREBOX_CENTER.x;
    scoring->y = SCOREBOX_CENTER.y;

    char *score = malloc(100*sizeof(char));
    char *highscore = malloc(100*sizeof(char));

    body_t *doodle;

    while (!sdl_is_done(scene)) {
        if (strcmp(scene_get_info(scene), "game") == 0 && strcmp(body_get_info(scene_get_body(scene, 0)), "doodle") != 0) {
            char *restart_info = malloc(10*sizeof(char));
            strcpy(restart_info, "restart");
            scene_set_next_info(scene, restart_info);
        }
        if (strcmp(scene_get_info(scene), scene_get_next_info(scene)) != 0) {
            if (strcmp(scene_get_next_info(scene), "game") == 0) {
                center->x = SCREEN_DIMENSIONS.x/2;
                center->y = SCREEN_DIMENSIONS.y/2;
                sdl_set_center(*center);
                star_score += scene_stars(scene);
                score = malloc(100*sizeof(char));
                reset_clock();
                scene_free(scene);
                scene = make_game_scene();
                use_inventory(scene);
                doodle = scene_get_body(scene, 0);
            }
            else if (strcmp(scene_get_next_info(scene), "start") == 0) {
                center->x = SCREEN_DIMENSIONS.x/2;
                center->y = SCREEN_DIMENSIONS.y/2;
                sdl_set_center(*center);
                star_score += scene_stars(scene);
                scene_free(scene);
                scene = make_start_scene();
            }
            else if (strcmp(scene_get_next_info(scene), "restart") == 0) {
                center->x = SCREEN_DIMENSIONS.x/2;
                center->y = SCREEN_DIMENSIONS.y/2;
                sdl_set_center(*center);
                star_score += scene_stars(scene);
                scene_free(scene);
                scene = make_restart_scene(score, highscore);
            }
            else if (strcmp(scene_get_next_info(scene), "shop") == 0) {
                center->x = SCREEN_DIMENSIONS.x/2;
                center->y = SCREEN_DIMENSIONS.y/2;
                sdl_set_center(*center);
                scene_free(scene);
                scene = make_shop_scene();
            }
            else if (strcmp(scene_get_next_info(scene), "shop exit") == 0) {
                scene_free(scene);
                scene = make_shop_exit_scene();
            }
            else if (strcmp(scene_get_next_info(scene), "shop fail") == 0) {
                scene_free(scene);
                scene = make_failed_purchase_scene();
            }
            else if (strcmp(scene_get_next_info(scene), "settings") == 0) {
                star_score += scene_stars(scene);
                scene_free(scene);
                scene = make_settings_scene();
            }
            else if (strcmp(scene_get_next_info(scene), "instructions") == 0) {
                scene_free(scene);
                scene = make_instructions_scene();
            }
        }
        if (strcmp(scene_get_info(scene), "game") == 0) {
            game_main(scene, doodle, star_timer, powerup_timer, center, score);
        }
        if (strcmp(scene_get_info(scene), "instructions") == 0) {
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "start") == 0) {
            start_timer++;
            start_main(scene, start_timer);
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "restart") == 0) {
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "shop") == 0) {
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "shop exit") == 0) {
            if (shop_timer == SHOP_TIMER){
                char *start_info = malloc(6*sizeof(char));
                strcpy(start_info, "start");
                scene_set_next_info(scene, start_info);
                shop_timer = 0;
            }
            shop_timer++;
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "shop fail") == 0) {
            if (shop_timer == SHOP_TIMER){
                char *shop_info = malloc(5*sizeof(char));
                strcpy(shop_info, "shop");
                scene_set_next_info(scene, shop_info);
                shop_timer = 0;
            }
            shop_timer++;
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "settings") == 0) {
            sdl_render_scene(scene);
        }
    }
    free(center);
    free(score);
    free(highscore);
    free(scoring);
    free_sounds();
    scene_free(scene);
    free(star_timer);
    free(powerup_timer);
    SDL_Renderer *renderer = get_renderer();
    SDL_Quit();
    SDL_DestroyRenderer(renderer);
    return 0;
}
