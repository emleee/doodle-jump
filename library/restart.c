#include "restart.h"
#include "sdl_wrapper.h"
#include "game_sprites.h"
#include "constants.h"

const vector_t SCORE_CENTER = {.x = 360, .y = 125};
const vector_t HIGHSCORE_CENTER = {.x = 360, .y = 240};
const vector_t RESTART_BUTTON = {.x = 360, .y = 400};
const vector_t HOME_BUTTON = {.x = 360, .y = 565};

char *get_high_score(char *highscore) {
    FILE* file = fopen("highscore.txt", "r");
    if (!file) {
        return NULL;
    }
    char *buffer = malloc(100*sizeof(char));
    strcpy(highscore, "High Score: ");
    fgets(buffer, sizeof(buffer), file);
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

scene_t *make_restart_scene(char *score, char *highscore) {
    char *scene_info = malloc(8*sizeof(char));
    strcpy(scene_info, "restart");
    scene_t *scene = scene_init_with_info(scene_info, free);

    vector_t *score_point = malloc(sizeof(vector_t));
    score_point->x = SCORE_CENTER.x;
    score_point->y = SCORE_CENTER.y;
    text_t *score_text = text_create(score, TEXT_COLOR, 22, score_point);
    scene_add_text(scene, score_text);

    vector_t *highscore_point = malloc(sizeof(vector_t));
    highscore_point->x = HIGHSCORE_CENTER.x;
    highscore_point->y = HIGHSCORE_CENTER.y;
    get_high_score(highscore);
    text_t *highscore_text = text_create(highscore, TEXT_COLOR, 22, highscore_point);
    scene_add_text(scene, highscore_text);
    
    body_t *background = make_background_body("PNGs/Restart_Background.png",(vector_t){.x = 0, .y = SCREEN_DIMENSIONS.y});
    scene_add_body(scene, background);
    return scene;
}