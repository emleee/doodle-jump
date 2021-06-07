#include "constants.h"

const vector_t SCREEN_DIMENSIONS = {.x = 720, .y = 960};

// Doodle parameters
const double DOODLE_WIDTH = 96.0;
const double DOODLE_HEIGHT = 148.0;
const double DOODLE_MASS = 10;
const rgb_color_t DOODLE_COLOR = {.r = 176.0/255, .g = 128.0/255, .b = 124.0/255};
const vector_t START_VELOCITY = {.x = 0, .y = 300};
const double MAX_JUMP = 295.0;

const double G = -150.0;

// Star parameters
const int NUM_STAR_POINTS = 5;
const int STAR_RADIUS = 17;
const rgb_color_t STAR_COLOR = {.r = 1, .g = 1, .b = 0};
const int STAR_OFFSET = 40;

const rgb_color_t TEXT_COLOR = {.r = 0, .g = 0, .b = 0};

const rgb_color_t SPRITE_COLOR = {.r = 1, .g = 1, .b = 1};
