#include <math.h>
#include <stdlib.h>
#include "polygon.h"

const double CIRCLE_POINTS = 20;

double polygon_area(list_t *polygon) {
    size_t num_sides = list_size(polygon);
    double area = 0.0;

    for (size_t i = 0; i < num_sides; i++) {
        vector_t *pt1 = list_get(polygon, i % num_sides);
        vector_t *pt2 = list_get(polygon, (i+1) % num_sides);
        area += vec_cross(*pt1, *pt2);
    }
    area = sqrt(pow(area, 2.0)) / 2;

    return area;
}

vector_t polygon_centroid(list_t *polygon) {
    size_t num_sides = list_size(polygon);
    double area = 0.0;
    double cx = 0.0;
    double cy = 0.0;

    // perform Shoelace formula
    for (size_t i = 0; i < num_sides; i++) {
        vector_t *pt1 = list_get(polygon, i % num_sides);
        vector_t *pt2 = list_get(polygon, (i+1) % num_sides);
        cx += (pt1->x + pt2->x) * vec_cross(*pt1, *pt2);
        cy += (pt1->y + pt2->y) * vec_cross(*pt1, *pt2);
        area += vec_cross(*pt1, *pt2);
    }
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

list_t *make_rectangle(vector_t bottom_left, double width, double height) {
    list_t *rectangle = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));

    *v = (vector_t) {bottom_left.x, bottom_left.y};
    list_add(rectangle, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {bottom_left.x + width, bottom_left.y};
    list_add(rectangle, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {bottom_left.x + width, bottom_left.y + height};
    list_add(rectangle, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {bottom_left.x, bottom_left.y + height};
    list_add(rectangle, v);

    return rectangle;
}

list_t *make_circle(double radius) {
    list_t *circle = list_init(CIRCLE_POINTS, free);
    for (int i = 0; i < CIRCLE_POINTS; i++) {
        vector_t *pt = malloc(sizeof(vector_t));
        pt->x = radius * cos(2 * M_PI * i / CIRCLE_POINTS + M_PI / 2);
        pt->y = radius * sin(2 * M_PI * i / CIRCLE_POINTS + M_PI / 2);
        list_add(circle, pt);
    }
    return circle;
}