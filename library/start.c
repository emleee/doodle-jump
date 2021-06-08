#include "start.h"
#include "constants.h"

const vector_t START_BUTTON = {.x = 360, .y = 245};
const vector_t SHOP_BUTTON = {.x = 360, .y = 425};
const vector_t SETTINGS_BUTTON = {.x = 360, .y = 588};

const int START_TIMER = 80;
const int START_TIMER_OFFSET = 320;
const int START1_IDX = 0;
const int START2_IDX = 1;
const int START3_IDX = 2;

scene_t *make_start_scene() {
    char *scene_info = malloc(6*sizeof(char));
    strcpy(scene_info, "start");
    scene_t *scene = scene_init_with_info(scene_info, free);

    body_t *background = make_background_body("PNGs/Start_Background_1.png", (vector_t){.x = 0, .y = SCREEN_DIMENSIONS.y});
    scene_add_body(scene, background);
    sprite_t *background1 = body_get_sprite(background);
    sprite_t *background2 = create_sprite("PNGs/Start_Background_2.png", 720, 960);
    sprite_set_center(background2, (vector_t){.x=360,.y=480});
    sprite_t *background3 = create_sprite("PNGs/Start_Background_3.png", 720, 960);
    sprite_set_center(background3, (vector_t){.x=360,.y=480});
    scene_add_sprite(scene, background1);
    scene_add_sprite(scene, background2);
    scene_add_sprite(scene, background3);

    return scene;
}

void start_mouse_click(scene_t *scene, int x, int y, double button_x_radius, double button_y_radius){
    if (x < (START_BUTTON.x + button_x_radius) && x > (START_BUTTON.x - button_x_radius)) {
        if (y < (START_BUTTON.y + button_y_radius) && y > (START_BUTTON.y - button_y_radius)) {
            if (first_time_play()) {
                char *game_info = malloc(13*sizeof(char));
                strcpy(game_info, "instructions");
                scene_set_next_info(scene, game_info);
            }
            else {
                char *game_info = malloc(5*sizeof(char));
                strcpy(game_info, "game");
                scene_set_next_info(scene, game_info);
            }
        }
    }
    if (x < (SETTINGS_BUTTON.x + button_x_radius) && x > (SETTINGS_BUTTON.x - button_x_radius)) {
        if (y < (SETTINGS_BUTTON.y + button_y_radius) && y > (SETTINGS_BUTTON.y - button_y_radius)) {
            char *settings_info = malloc(9*sizeof(char));
            strcpy(settings_info, "settings");
            scene_set_next_info(scene, settings_info);
        }
    }
    if (x < (SHOP_BUTTON.x + button_x_radius) && x > (SHOP_BUTTON.x - button_x_radius)) {
        if (y < (SHOP_BUTTON.y + button_y_radius) && y > (SHOP_BUTTON.y - button_y_radius)) {
            char *shop_info = malloc(5*sizeof(char));
            strcpy(shop_info, "shop");
            scene_set_next_info(scene, shop_info);
        }
    }
}

void start_main (scene_t *scene, int timer) {
    if (timer % START_TIMER_OFFSET < START_TIMER) {
        body_set_sprite(scene_get_body(scene, 0), scene_get_sprite(scene, START1_IDX));
    }
    else if (timer % START_TIMER_OFFSET < 2*START_TIMER) {
        body_set_sprite(scene_get_body(scene, 0), scene_get_sprite(scene, START2_IDX));
    }
    else if (timer % START_TIMER_OFFSET < 3*START_TIMER) {
        body_set_sprite(scene_get_body(scene, 0), scene_get_sprite(scene, START3_IDX));
    }
    else  {
        body_set_sprite(scene_get_body(scene, 0), scene_get_sprite(scene, START2_IDX));
    }
}