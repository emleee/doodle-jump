#include "platforms.h"
#include "preferences.h"
#include "body.h"
#include "sdl_wrapper.h"
#include "game_sprites.h"
#include "constants.h"

bool SOUND_EFFECTS = true;
bool SCORE_MARKER = true;
bool POWERUPS = true;

const size_t SOUND_IDX = 6;
const size_t SCORE_IDX = 7;
const size_t POWERUP_IDX = 8;

const vector_t PREF_TITLE = {.x = 360, .y = 220};
const vector_t PREF_DESCRIPTION = {.x = 360, .y = 250};
const vector_t SOUND = {.x = 290, .y = 350};
const vector_t SCORE = {.x = 290, .y = 450};
const vector_t POWERUP = {.x = 290, .y = 550};
const vector_t PREF_HOME_BUTTON = {.x = 360, .y = 650};
const double PREF_BUTTON_OFFSET = 180.0;

scene_t *make_settings_scene() { // add something to keep track score vs high score and the falling/sad doodle
    char *scene_info = malloc(9*sizeof(char));
    strcpy(scene_info, "settings");
    scene_t *scene = scene_init_with_info(scene_info, free);
    rgb_color_t color = {.r = 0, .g = 0, .b = 0};

    vector_t *title_point = malloc(sizeof(vector_t));
    title_point->x = PREF_TITLE.x;
    title_point->y = PREF_TITLE.y;
    text_t *title_text = text_create("Settings", color, 28, title_point);
    scene_add_text(scene, title_text);

    vector_t *description_point = malloc(sizeof(vector_t));
    description_point->x = PREF_DESCRIPTION.x;
    description_point->y = PREF_DESCRIPTION.y;
    text_t *description_text = text_create("Click ON or OFF to toggle settings.", color, 18, description_point);
    scene_add_text(scene, description_text);

    vector_t *point2 = malloc(sizeof(vector_t));
    point2->x = PREF_HOME_BUTTON.x;
    point2->y = PREF_HOME_BUTTON.y;
    text_t *text2 = text_create("Back to Home", color, 22, point2);
    scene_add_text(scene, text2);

    body_t *background1 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = SCREEN_DIMENSIONS.y});
    body_t *background2 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = 2*SCREEN_DIMENSIONS.y});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);

    vector_t *sound_point = malloc(sizeof(vector_t));
    sound_point->x = SOUND.x;
    sound_point->y = SOUND.y;
    text_t *sound_text = text_create("Sound Effects", color, 22, sound_point);
    scene_add_text(scene, sound_text);

    vector_t *score_point = malloc(sizeof(vector_t));
    score_point->x = SCORE.x;
    score_point->y = SCORE.y;
    text_t *score_text = text_create("Score Markers", color, 22, score_point);
    scene_add_text(scene, score_text);

    vector_t *powerup_point = malloc(sizeof(vector_t));
    powerup_point->x = POWERUP.x;
    powerup_point->y = POWERUP.y;
    text_t *powerup_text = text_create("Powerup", color, 22, powerup_point);
    scene_add_text(scene, powerup_text);

    vector_t *sound_button_point = malloc(sizeof(vector_t));
    sound_button_point->x = SOUND.x + PREF_BUTTON_OFFSET;
    sound_button_point->y = SOUND.y;
    bool sound_pref = get_sound_preference();
    if (sound_pref) {
        text_t *sound = text_create("ON", color, 22, sound_button_point);
        scene_add_text(scene, sound);
    }
    else if (!sound_pref) {
        text_t *sound = text_create("OFF", color, 22, sound_button_point);
        scene_add_text(scene, sound);
    }
    vector_t *score_button_point = malloc(sizeof(vector_t));
    score_button_point->x = SCORE.x + PREF_BUTTON_OFFSET;
    score_button_point->y = SCORE.y;
    bool score_pref = get_score_preference();
    if (score_pref) {
        text_t *score = text_create("ON", color, 22, score_button_point);
        scene_add_text(scene, score);
    }
    else if (!score_pref) {
        text_t *score = text_create("OFF", color, 22, score_button_point);
        scene_add_text(scene, score);
    }
    vector_t *powerup_button_point = malloc(sizeof(vector_t));
    powerup_button_point->x = POWERUP.x + PREF_BUTTON_OFFSET; 
    powerup_button_point->y = POWERUP.y;
    bool powerup_pref = get_powerup_preference();
    if (powerup_pref) {
        text_t *powerup = text_create("ON", color, 22, powerup_button_point);
        scene_add_text(scene, powerup);
    }
    else if (!powerup_pref) {
        text_t *powerup = text_create("OFF", color, 22, powerup_button_point);
        scene_add_text(scene, powerup);
    }
    return scene;
}

bool get_sound_preference() {
    FILE* file = fopen("preferences.txt", "r");
    if (!file) {
        return SOUND_EFFECTS;
    }
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "Sound: ON\n") == 0) {
            SOUND_EFFECTS = true;
        }
        else if (strcmp(line, "Sound: OFF\n") == 0) {
            SOUND_EFFECTS = false;
        }
    }
    fclose(file);
    return SOUND_EFFECTS;
}

bool get_score_preference() {
    FILE* file = fopen("preferences.txt", "r");
    if (!file) {
        return SCORE_MARKER;
    }
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "Score: ON\n") == 0) {
            SCORE_MARKER = true;
        }
        else if (strcmp(line, "Score: OFF\n") == 0) {
            SCORE_MARKER = false;
        }
    }
    fclose(file);
    return SCORE_MARKER;
}

bool get_powerup_preference() {
    FILE* file = fopen("preferences.txt", "r");
    if (!file) {
        return POWERUPS;
    }
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "Powerup: ON\n") == 0) {
            POWERUPS = true;
        }
        else if (strcmp(line, "Powerup: OFF\n") == 0) {
            POWERUPS = false;
        }
    }
    fclose(file);
    return POWERUPS;
}

void switch_sound_preferences() {
    SOUND_EFFECTS = !SOUND_EFFECTS;
}

void switch_score_preferences() {
    SCORE_MARKER = !SCORE_MARKER;
}

void switch_powerup_preferences() {
    POWERUPS = !POWERUPS;
}

void update_preferences() {
    FILE *file = fopen("preferences.txt", "w");
    if (!file) {
        return;
    }
    if (SOUND_EFFECTS) {
        fprintf(file,"Sound: ON\n");
    }
    else {
        fprintf(file,"Sound: OFF\n");
    }
    if (SCORE_MARKER) {
        fprintf(file,"Score: ON\n");
    }
    else {
        fprintf(file,"Score: OFF\n");
    }
    if (POWERUPS) {
        fprintf(file,"Powerup: ON\n");
    }
    else {
        fprintf(file,"Powerup: OFF\n");
    }
    fclose(file);
}

void settings_mouse_click (scene_t *scene, int x, int y, double button_x_radius, double button_y_radius) {
    if (x < ((SOUND.x + PREF_BUTTON_OFFSET) + button_x_radius) && x > ((SOUND.x + PREF_BUTTON_OFFSET) - button_x_radius)) {
        if (y < (SOUND.y + button_y_radius) && y > (SOUND.y - button_y_radius)) {
            switch_sound_preferences();
            update_preferences();
        }
        else if (y < (SCORE.y + button_y_radius) && y > (SCORE.y - button_y_radius)) {
            switch_score_preferences();
            update_preferences();
        }
        else if (y < (POWERUP.y + button_y_radius) && y > (POWERUP.y - button_y_radius)) {
            switch_powerup_preferences();
            update_preferences();
        }
        rgb_color_t color = {.r = 0, .g = 0, .b = 0};
        text_t *sound = scene_get_text(scene, SOUND_IDX);
        text_t *score = scene_get_text(scene, SCORE_IDX);
        text_t *powerup = scene_get_text(scene, POWERUP_IDX);
        scene_remove_text(scene, sound);
        scene_remove_text(scene, score);
        scene_remove_text(scene, powerup);
        vector_t *sound_point = malloc(sizeof(vector_t));
        sound_point->x = SOUND.x + PREF_BUTTON_OFFSET;
        sound_point->y = SOUND.y;
        bool sound_pref = get_sound_preference();
        if (sound_pref) {
            sound = text_create("ON", color, 22, sound_point);
        }
        else if (!sound_pref){
            sound = text_create("OFF", color, 22, sound_point);
        }
        vector_t *score_point = malloc(sizeof(vector_t));
        score_point->x = SCORE.x + PREF_BUTTON_OFFSET;
        score_point->y = SCORE.y;
        bool score_pref = get_score_preference();
        if (score_pref) {
            score = text_create("ON", color, 22, score_point);
        }
        else if (!score_pref) {
            score = text_create("OFF", color, 22, score_point);
        }
        vector_t *powerup_point = malloc(sizeof(vector_t));
        powerup_point->x = POWERUP.x + PREF_BUTTON_OFFSET;
        powerup_point->y = POWERUP.y;
        bool powerup_pref = get_powerup_preference();
        if (powerup_pref) {
            powerup = text_create("ON", color, 22, powerup_point);
        }
        else if (!powerup_pref) { //make else ifs elses
            powerup = text_create("OFF", color, 22, powerup_point);
        }
        scene_add_text(scene, sound);
        scene_add_text(scene, score);
        scene_add_text(scene, powerup);
    }
    if (x < (PREF_HOME_BUTTON.x + button_x_radius) && x > (PREF_HOME_BUTTON.x - button_x_radius)) {
        if (y < (PREF_HOME_BUTTON.y + button_y_radius) && y > (PREF_HOME_BUTTON.y - button_y_radius)) {
            char *start_info = malloc(6*sizeof(char));
            strcpy(start_info, "start");
            scene_set_next_info(scene, start_info);
        }
    }
}