#include <stddef.h>
#include <math.h>
#include "polygon.h"
#include <assert.h>
#include <stdlib.h>

double polygon_area(list_t *polygon) {
    size_t num_sides = list_size(polygon);
    double area = 0.0;
    for (size_t i = 0; i < (num_sides-1); i++) {
        vector_t *pti = list_get(polygon, i);
        vector_t *pti2 = list_get(polygon, i+1);
        area += vec_cross(*pti, *pti2);
    }
    vector_t *pt1 = list_get(polygon, 0);
    vector_t *ptn = list_get(polygon, num_sides-1);
    area += vec_cross(*ptn, *pt1);
    area = sqrt(pow(area, 2.0)) / 2;
    return area;
}

vector_t polygon_centroid(list_t *polygon) {
    size_t num_sides = list_size(polygon);
    double area = 0.0;
    double cx = 0.0;
    double cy = 0.0;

    // perform Shoelace formula
    for (size_t i = 0; i < (num_sides-1); i++) {
        vector_t *pti = list_get(polygon, i);
        vector_t *pti2 = list_get(polygon, i+1);
        cx += (pti->x + pti2->x) * vec_cross(*pti, *pti2);
        cy += (pti->y + pti2->y) * vec_cross(*pti, *pti2);
        area += vec_cross(*pti, *pti2);
    }
    vector_t *pt1 = list_get(polygon, 0);
    vector_t *ptn = list_get(polygon, (num_sides - 1));
    cx += (ptn->x + pt1->x) * vec_cross(*ptn, *pt1);
    cy += (ptn->y + pt1->y) * vec_cross(*ptn, *pt1);
    area += vec_cross(*ptn, *pt1);

    area /= 2;
    cx /= 6 * area;
    cy /= 6 * area;

    vector_t centroid;
    centroid.x = cx;
    centroid.y = cy;
    return centroid;
}

void polygon_translate(list_t *polygon, vector_t translation) {
    size_t num_sides = list_size(polygon);
    for (size_t i = 0; i < num_sides; i++) {
        vector_t *poly_vec = list_get(polygon, i);
        *poly_vec = vec_add(*poly_vec, translation);
    }
}

void polygon_rotate(list_t *polygon, double angle, vector_t point) {
    size_t num_sides = list_size(polygon);
    polygon_translate(polygon, vec_negate(point));
    for (size_t i = 0; i < num_sides; i++) {
        vector_t *poly_vec = list_get(polygon, i);
        *poly_vec = vec_rotate(*poly_vec, angle);
    }
    polygon_translate(polygon, point);
}