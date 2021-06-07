#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include "vector.h"
#include "color.h"

extern const vector_t SCREEN_DIMENSIONS;

const double DOODLE_WIDTH;
const double DOODLE_HEIGHT;
const double DOODLE_MASS;
const rgb_color_t DOODLE_COLOR;
const vector_t START_VELOCITY;
const double MAX_JUMP;

const double G;

const int NUM_STAR_POINTS;
const int STAR_RADIUS;
const rgb_color_t STAR_COLOR;
const int STAR_OFFSET;

const rgb_color_t TEXT_COLOR;

extern const rgb_color_t SPRITE_COLOR;

#endif // #ifndef __CONSTANTS_H__