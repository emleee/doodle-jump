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
#include "preferences.h"

const double PREF_WIDTH = 720.0;
const double PREF_HEIGHT = 960.0;
const double PREF_BUTTON_OFFSET = 100;
bool SOUND_EFFECTS = true;
bool SCORE_MARKER = true;


const double PREF_BUTTON_X_RADIUS = 125;
const double PREF_BUTTON_Y_RADIUS = 75;
const size_t SOUND_IDX = 5;
const size_t SCORE_IDX = 6;

scene_t *make_settings_scene() { // add something to keep track score vs high score and the falling/sad doodle
    char *scene_info = malloc(9*sizeof(char));
    strcpy(scene_info, "settings");
    scene_t *scene = scene_init_with_info(scene_info, free);
    rgb_color_t color = {.r = 0, .g = 0, .b = 0};

    vector_t *title_point = malloc(sizeof(vector_t));
    title_point->x = 250; // remove magic numbers
    title_point->y = 50;
    text_t *title_text = text_create("Settings", color, 28, title_point);
    scene_add_text(scene, title_text);

    vector_t *info_point = malloc(sizeof(vector_t));
    info_point->x = 200; // remove magic numbers
    info_point->y = 80;
    text_t *info_text = text_create("Click ON or OFF to toggle settings.", color, 18, info_point);
    scene_add_text(scene, info_text);

    vector_t *point2 = malloc(sizeof(vector_t));
    point2->x = 250; // remove magic numbers
    point2->y = 500;
    text_t *text2 = text_create("Back to Home", color, 22, point2);
    scene_add_text(scene, text2);

    body_t *background1 = make_background_body((vector_t){.x = 0, .y = PREF_HEIGHT});
    body_t *background2 = make_background_body((vector_t){.x = 0, .y = 2*PREF_HEIGHT});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);

    vector_t *sound_point = malloc(sizeof(vector_t));
    sound_point->x = 220; // remove magic numbers
    sound_point->y = 300;
    text_t *sound_text = text_create("Sound Effects", color, 22, sound_point);
    scene_add_text(scene, sound_text);

    vector_t *score_point = malloc(sizeof(vector_t));
    score_point->x = 220; // remove magic numbers
    score_point->y = 300 + PREF_BUTTON_OFFSET;
    text_t *score_text = text_create("Score Markers", color, 22, score_point);
    scene_add_text(scene, score_text);

    vector_t *on1_point = malloc(sizeof(vector_t));
    on1_point->x = 400; // remove magic numbers
    on1_point->y = 300;
    bool sound_pref = get_sound_preference();
    if (sound_pref) {
        text_t *sound = text_create("ON", color, 22, on1_point);
        scene_add_text(scene, sound);
    }
    if (!sound_pref) {
        text_t *sound = text_create("OFF", color, 22, on1_point);
        scene_add_text(scene, sound);
    }
    vector_t *on2_point = malloc(sizeof(vector_t));
    on2_point->x = 400; // remove magic numbers
    on2_point->y = 300 + PREF_BUTTON_OFFSET;
    bool score_pref = get_score_preference();
    if (score_pref) {
        text_t *score = text_create("ON", color, 22, on2_point);
        scene_add_text(scene, score);
    }
    if (!score_pref) {
        text_t *score = text_create("OFF", color, 22, on2_point);
        scene_add_text(scene, score);
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
            // printf("sound is true");
            SOUND_EFFECTS = true;
        }
        else if (strcmp(line, "Sound: OFF\n") == 0) {
            // printf("sound is false");
            SOUND_EFFECTS = false;
        }
    }
    // printf("sound is closed");
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
            // printf("sound is true");
            SCORE_MARKER = true;
        }
        else if (strcmp(line, "Score: OFF\n") == 0) {
            // printf("score is false");
            SCORE_MARKER = false;
        }
    }
    fclose(file);
    // printf("score is closed");
    return SCORE_MARKER;
}

void switch_sound_preferences () {
    SOUND_EFFECTS = !SOUND_EFFECTS;
}

void switch_score_preferences () {
    SCORE_MARKER = !SCORE_MARKER;
}

void update_preferences() {
    FILE *fp = fopen("preferences.txt", "w"); // opening of file
    if (!fp) {
        return;
    }
    if (SOUND_EFFECTS) {
        fprintf(fp,"Sound: ON\n");
    }
    else if (!SOUND_EFFECTS) {
        fprintf(fp,"Sound: OFF\n");
    }
    if (SCORE_MARKER) {
        fprintf(fp,"Score: ON\n");
    }
    else if (!SCORE_MARKER) {
        fprintf(fp,"Score: OFF\n");
    }
    fclose(fp);
}

void settings_mouse_click (scene_t *scene, int x, int y) {
    if (x < (400 + PREF_BUTTON_X_RADIUS) && x > (400 - PREF_BUTTON_X_RADIUS)) {
        rgb_color_t color = {.r = 0, .g = 0, .b = 0};
        if (y < (300 + PREF_BUTTON_Y_RADIUS) && y > (300 - PREF_BUTTON_Y_RADIUS)) {
            switch_sound_preferences();
            update_preferences();
        }
        else if (y < (400 + PREF_BUTTON_Y_RADIUS) && y > (400 - PREF_BUTTON_Y_RADIUS)) {
            switch_score_preferences();
            update_preferences();
        }
        else if (y < (500 + PREF_BUTTON_Y_RADIUS) && y > (500 - PREF_BUTTON_Y_RADIUS)) {
            char *start_info = malloc(6*sizeof(char));
            strcpy(start_info, "start");
            scene_set_next_info(scene, start_info);
        }
        text_t *sound = scene_get_text(scene, SOUND_IDX);
        scene_remove_text(scene, sound);
        text_t *score = scene_get_text(scene, SCORE_IDX);
        scene_remove_text(scene, score);
        vector_t *on1_point = malloc(sizeof(vector_t));
        on1_point->x = 400; // remove magic numbers
        on1_point->y = 300;
        bool sound_pref = get_sound_preference();
        if (sound_pref) {
            sound = text_create("ON", color, 22, on1_point);
        }
        if (!sound_pref) {
            sound = text_create("OFF", color, 22, on1_point);
        }
        vector_t *on2_point = malloc(sizeof(vector_t));
        on2_point->x = 400; // remove magic numbers
        on2_point->y = 300 + PREF_BUTTON_OFFSET;
        bool score_pref = get_score_preference();
        if (score_pref) {
            score = text_create("ON", color, 22, on2_point);
        }
        if (!score_pref) {
            score = text_create("OFF", color, 22, on2_point);
        }
        scene_add_text(scene, sound);
        scene_add_text(scene, score);
    }
}