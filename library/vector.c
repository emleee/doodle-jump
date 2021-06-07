#include <math.h>
#include "vector.h"

const vector_t VEC_ZERO = {.x = 0, .y = 0};

vector_t vec_add(vector_t v1, vector_t v2) {
    vector_t sum;
    sum.x = v1.x + v2.x;
    sum.y = v1.y + v2.y;
    return sum;
}

vector_t vec_subtract(vector_t v1, vector_t v2) {
    vector_t diff;
    diff.x = v1.x - v2.x;
    diff.y = v1.y - v2.y;
    return diff;
}

vector_t vec_negate(vector_t v) {
    vector_t neg;
    neg.x = -1 * v.x;
    neg.y = -1 * v.y;
    return neg;
}

vector_t vec_multiply(double scalar, vector_t v) {
    vector_t mult;
    mult.x = scalar * v.x;
    mult.y = scalar * v.y;
    return mult;
}

double vec_dot(vector_t v1, vector_t v2) {
    return (v1.x * v2.x) + (v1.y * v2.y);
}

double vec_cross(vector_t v1, vector_t v2) {
    return (v1.x * v2.y) - (v1.y * v2.x);
}

vector_t vec_rotate(vector_t v, double angle) {
    vector_t rot;
    rot.x = v.x*cos(angle) - v.y*sin(angle);
    rot.y = v.x*sin(angle) + v.y*cos(angle);
    return rot;
}