#include "restart.h"

const double RESTART_HEIGHT = 960.0;
const double VELOCITY_X = 600;

const vector_t RESTART_BUTTON = {.x = 250, .y = 400};
const vector_t HOME_BUTTON = {.x = 250, .y = 425};

void restart_mouse_click(scene_t *scene, int x, int y, double button_x_radius, double button_y_radius) {
    if (x < (RESTART_BUTTON.x + button_x_radius) && x > (RESTART_BUTTON.x - button_x_radius)) {
        if (y < (RESTART_BUTTON.y + button_y_radius) && y > (RESTART_BUTTON.y - button_y_radius)) {
            char *game_info = malloc(5*sizeof(char));
            strcpy(game_info, "game");
            scene_set_next_info(scene, game_info);
        }
    }
    if (x < (HOME_BUTTON.x + button_x_radius) && x > (HOME_BUTTON.x - button_x_radius)) {
        if (y < (HOME_BUTTON.y + button_y_radius) && y > (HOME_BUTTON.y - button_y_radius)) {
            char *start_info = malloc(6*sizeof(char));
            strcpy(start_info, "start");
            scene_set_next_info(scene, start_info);
        }
    }
}

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

    body_t *background1 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = RESTART_HEIGHT});
    body_t *background2 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = 2*RESTART_HEIGHT});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);
    return scene;
}