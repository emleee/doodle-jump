#include <math.h>
#include <stdlib.h>
#include "collision.h"

vector_t get_perp_line(vector_t *pt1, vector_t *pt2) {
    double y = pt1->y - pt2->y;
    double x = pt1->x - pt2->x;
    vector_t perp_line = {.x = -y, .y = x};
    double magnitude = vec_dot(perp_line, perp_line);
    magnitude = sqrt(magnitude);
    perp_line = vec_multiply(1/magnitude, perp_line);
    return perp_line;
}

list_t *polygon_projection(list_t *shape, vector_t perp_line) {
    list_t *projections = list_init(2, (free_func_t)free);
    double min = INFINITY;
    double max = -INFINITY;

    for (int i = 0; i < list_size(shape); i++) {
        double projection = vec_dot(*(vector_t *)list_get(shape, i), perp_line);
        if (projection < min) {
            min = projection;
        } 
        if (projection > max) {
            max = projection;
        }
    }

    double *min_proj = malloc(sizeof(double));
    double *max_proj = malloc(sizeof(double));
    *min_proj = min;
    *max_proj = max;
    list_add(projections, min_proj);
    list_add(projections, max_proj);
    return projections;
}

bool overlap(list_t *proj1, list_t *proj2) {
    double proj1_min = *(double *)list_get(proj1, 0);
    double proj1_max = *(double *)list_get(proj1, 1);
    double proj2_min = *(double *)list_get(proj2, 0);
    double proj2_max = *(double *)list_get(proj2, 1);
    if (proj1_max < proj2_min || proj2_max < proj1_min){
        return false;
    }
    return true;
}

double compute_overlap(list_t *proj1, list_t *proj2) {
    double overlap = INFINITY;
    double proj1_min = *(double *)list_get(proj1, 0);
    double proj1_max = *(double *)list_get(proj1, 1);
    double proj2_min = *(double *)list_get(proj2, 0);
    double proj2_max = *(double *)list_get(proj2, 1);
    if (proj1_max - proj2_min < overlap) {
        overlap = proj1_max - proj2_min;
    }
    if (proj2_max - proj1_min < overlap) {
        overlap = proj2_max - proj2_min;
    }
    return overlap;
}

collision_info_t find_collision(list_t *shape1, list_t *shape2) {
    bool collided = true;
    double min_overlap = INFINITY;
    vector_t axis;

    for (int i = 0; i < list_size(shape1); i++) {
        vector_t perp_line = get_perp_line(list_get(shape1, i), list_get(shape1, (i+1) % list_size(shape1)));
        list_t *proj1 = polygon_projection(shape1, perp_line);
        list_t *proj2 = polygon_projection(shape2, perp_line);
        if (!overlap(proj1, proj2)){
            collided = false;
        }
        else {
            if (compute_overlap(proj1, proj2) < min_overlap) {
                min_overlap = compute_overlap(proj1, proj2);
                axis = perp_line;
            }
        }
        list_free(proj1);
        list_free(proj2);
    }
    for (int i = 0; i < list_size(shape2) - 1; i++) {
        vector_t perp_line = get_perp_line(list_get(shape2, i), list_get(shape2, (i+1) % list_size(shape2)));
        list_t *proj1 = polygon_projection(shape1, perp_line);
        list_t *proj2 = polygon_projection(shape2, perp_line);
        if (!overlap(proj1, proj2)){
            collided = false;
        }
        else {
            if (compute_overlap(proj1, proj2) < min_overlap) {
                min_overlap = compute_overlap(proj1, proj2);
                axis = perp_line;
            }
        }
        list_free(proj1);
        list_free(proj2);
    }
    list_free(shape1);
    list_free(shape2);
    collision_info_t collision_info = {.collided = collided, .axis = axis};
    return collision_info;
}
