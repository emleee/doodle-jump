#include "restart.h"

const double RESTART_HEIGHT = 960.0;
const double RESTART_WIDTH = 720.0;

const vector_t RESTART_BUTTON = {.x = 250, .y = 465};
const vector_t HOME_BUTTON = {.x = 250, .y = 480};

char *get_high_score(char *highscore) {
    FILE* file = fopen("highscore.txt", "r");
    if (!file) {
        return NULL;
    }
    char *buffer = malloc(100*sizeof(char));
    strcpy(highscore, "High Score: ");
    fgets(buffer, 6, file);
    strcat(highscore, buffer);
    fclose(file);
    free(buffer);
    return highscore;
}

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

scene_t *make_restart_scene(char *score, char *highscore) { // add something to keep track score vs high score and the falling/sad doodle
    char *scene_info = malloc(8*sizeof(char));
    strcpy(scene_info, "restart");
    scene_t *scene = scene_init_with_info(scene_info, free);

    rgb_color_t color = {.r = 0, .g = 0, .b = 0};
    vector_t *point1 = malloc(sizeof(vector_t));
    point1->x = RESTART_WIDTH/2; // remove magic numbers
    point1->y = 200;
    text_t *text1 = text_create(score, color, 22, point1);
    scene_add_text(scene, text1);

    vector_t *highscore_point = malloc(sizeof(vector_t));
    highscore_point->x = RESTART_WIDTH/2; // remove magic numbers
    highscore_point->y = 300;
    get_high_score(highscore);
    text_t *highscore_text = text_create(highscore, color, 22, highscore_point);
    scene_add_text(scene, highscore_text);

    vector_t *point = malloc(sizeof(vector_t));
    point->x = RESTART_WIDTH/2; // remove magic numbers
    point->y = 400;
    text_t *text = text_create("Restart", color, 22, point);
    scene_add_text(scene, text);

    vector_t *point2 = malloc(sizeof(vector_t));
    point2->x = RESTART_WIDTH/2; // remove magic numbers
    point2->y = 500;
    text_t *text2 = text_create("Home", color, 22, point2);
    scene_add_text(scene, text2);

    body_t *background1 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = RESTART_HEIGHT});
    body_t *background2 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = 2*RESTART_HEIGHT});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);
    return scene;
}