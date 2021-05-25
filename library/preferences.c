#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "preferences.h"

bool SOUND_EFFECTS = true;
bool SCORE_MARKER = true;

bool get_sound_preference() {
    FILE* file = fopen("preferences.txt", "r"); 

    if (!file) {
        return;
    }

    char line[500];

    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "Sound: ON\n") == 0) {
            SOUND_EFFECTS = true;
            return true;
        }
        if (strcmp(line, "Sound: OFF\n") == 0) {
            SOUND_EFFECTS = false;
            return false;
        }
    }
    fclose(file);
    return SOUND_EFFECTS;
}

bool get_score_preference() {
    FILE* file = fopen("preferences.txt", "r"); 

    if (!file) {
        return;
    }

    char line[500];

    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "Score: ON\n") == 0) {
            SCORE_MARKER = true;
            return true;
        }
        if (strcmp(line, "Score: OFF\n") == 0) {
            SCORE_MARKER = false;
            return false;
        }
    }
    fclose(file);
    return SCORE_MARKER;
}

void switch_sound_preferences () {
    SOUND_EFFECTS = !SOUND_EFFECTS;
}

void switch_score_preferences () {
    SCORE_MARKER = !SCORE_MARKER;
}

void update_preferences() {
    FILE *fp; // declaration of file pointer
    fp =fopen("preferences.txt", "w"); // opening of file
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