#include "force_aux.h"
#include "collision.h"
#include "sdl_wrapper.h"
#include "preferences.h"
#include "forces.h"

const double BOOST = -300;
const double BOOST_POWERUP = 1000;
const double DOODLE_HEIGHT2 = 148.0;
const double SCREEN_HEIGHT2 = 960.0;

void magnet(void *a) {
    force_aux_t *aux = (force_aux_t *)a;
    double k = force_aux_get_constant(aux);
    list_t *bodies = force_aux_get_bodies(aux);
    body_t *body1 = list_get(bodies, 0);
    body_t *body2 = list_get(bodies, 1);
    vector_t centroid1 = body_get_centroid(body1);
    vector_t centroid2 = body_get_centroid(body2);
    if (fabs(centroid2.y - centroid1.y) > SCREEN_HEIGHT2/2) {
        return;
    }
    vector_t between = vec_subtract(centroid2, centroid1);
    double distance = sqrt(vec_dot(between, between));
    vector_t force = vec_multiply(-1 * k * body_get_mass(body1) * body_get_mass(body2) / distance / distance / distance, between);
    body_add_force(body2, force);
}

void create_magnetic_force(scene_t *scene, double M, body_t *body1, body_t *body2) {
    force_aux_t *aux = force_aux_init(M);
    list_t *bodies1 = list_init(2, NULL);
    list_t *bodies2 = list_init(2, NULL);
    list_add(bodies1, body1);
    list_add(bodies1, body2);
    list_add(bodies2, body1);
    list_add(bodies2, body2);
    force_aux_set_bodies(aux, bodies1);
    scene_add_bodies_force_creator(scene, magnet, aux, bodies2, (free_func_t)force_aux_free);
}

void downward_gravity(void *a) {
    force_aux_t *aux = (force_aux_t *)a;
    double G = force_aux_get_constant(aux);
    vector_t acceleration = {.x = 0, .y = G};
    list_t *bodies = force_aux_get_bodies(aux);
    body_t *body = list_get(bodies, 0);
    double mass = body_get_mass(body);
    body_add_force(body, vec_multiply(mass, acceleration));
}

void create_downward_gravity(scene_t *scene, double G, body_t *body) {
    force_aux_t *aux = force_aux_init(G);
    list_t *bodies = list_init(1, NULL);
    list_t *bodies2 = list_init(1, NULL);
    list_add(bodies, body);
    list_add(bodies2, body);
    force_aux_set_bodies(aux, bodies);
    scene_add_bodies_force_creator(scene, downward_gravity, aux, bodies2, (free_func_t)force_aux_free);
}

void free_collision_package(collision_package_t *package) {
    package->freer(package->aux);
    free(package);
}

void create_collision(scene_t *scene, body_t *body1, body_t *body2, collision_handler_t handler, void *aux, free_func_t freer, force_creator_t collided) {
    list_t *bodies = list_init(2, NULL);
    list_add(bodies, body1);
    list_add(bodies, body2);
    collision_package_t *a = malloc(sizeof(collision_package_t));
    a->aux = aux;
    a->handler = handler;
    a->freer  = freer;
    a->bodies = bodies;
    a->collided = false;
    scene_add_bodies_force_creator(scene, collided, a, bodies, (free_func_t)free_collision_package);
}

void collided(void *a) {
    force_aux_t *aux = ((collision_package_t *)a)->aux;
    list_t *bodies = force_aux_get_bodies(aux);
    body_t *body1 = list_get(bodies, 0);
    body_t *body2 = list_get(bodies, 1);
    if (find_collision(body_get_shape(body1), body_get_shape(body2)).collided) {
        if (((collision_package_t *)a)->collided) {
            return;
        }
        ((collision_package_t *)a)->collided = true;
        vector_t axis = find_collision(body_get_shape(body1), body_get_shape(body2)).axis;
        ((collision_package_t *)a)->handler(body1, body2, axis, aux);
    }
    else {
        ((collision_package_t *)a)->collided = false;
    }
}

void platform_collided(void *a) {
    force_aux_t *aux = ((collision_package_t *)a)->aux;
    list_t *bodies = force_aux_get_bodies(aux);
    body_t *body1 = list_get(bodies, 0);
    body_t *body2 = list_get(bodies, 1);
    if (find_collision(body_get_shape(body1), body_get_shape(body2)).collided) {
        list_t *points1 = body_get_shape(body1);
        double min = body_get_centroid(body1).y;
        for (int i = 0; i < list_size(points1); i++) {
            double val = ((vector_t *)list_get(points1, i))->y;
            if (val < min) {
                min = val;
            }
        }
        list_t *points2 = body_get_shape(body2);
        double max = body_get_centroid(body2).y;
        for (int i = 0; i < list_size(points2); i++) {
            double val = ((vector_t *)list_get(points2, i))->y;
            if (val > max) {
                max = val;
            }
        }
        vector_t axis = find_collision(body_get_shape(body1), body_get_shape(body2)).axis;
        if ((max > min && min < body_get_centroid(body2).y) || body_get_velocity(body1).y > 0 || round(axis.x) != 0 || round(axis.y) != -1) {
            // printf("%f %f %f\n", max, min, body_get_velocity(body1).y);
            ((collision_package_t *)a)->collided = false;
            return;
        }
        if (((collision_package_t *)a)->collided) {
            return;
        }
        ((collision_package_t *)a)->collided = true;
        vector_t centroid = body_get_centroid(body1);
        centroid.y = max + DOODLE_HEIGHT2/2;
        body_set_centroid(body1, centroid);
        ((collision_package_t *)a)->handler(body1, body2, axis, aux);
        if (get_sound_preference()) {
            play_jump();
        }
        list_free(points1);
        list_free(points2);
    }
    else {
        ((collision_package_t *)a)->collided = false;
    }
}

void destructive_collision(body_t *body1, body_t *body2, vector_t *axis, void *aux) {
    if (find_collision(body_get_shape(body1), body_get_shape(body2)).collided) {
        body_remove(body1);
        body_remove(body2);
    }
}

void create_destructive_collision(scene_t *scene, body_t *body1, body_t *body2) {
    force_aux_t *aux = force_aux_init(0);
    list_t *bodies = list_init(2, NULL);
    list_add(bodies, body1);
    list_add(bodies, body2);
    force_aux_set_bodies(aux, bodies);
    create_collision(scene, body1, body2, (collision_handler_t)destructive_collision, aux, (free_func_t)force_aux_free, collided);
}

void platform_collision(body_t *body1, body_t *body2, vector_t axis, void *aux) {
    force_aux_t *a = (force_aux_t *)aux;
    double c = force_aux_get_constant(a);
    double mass1 = body_get_mass(body1);
    double mass2 = body_get_mass(body2);
    vector_t v1 = body_get_velocity(body1);
    vector_t v2 = body_get_velocity(body2);
    double impulse;
    if (mass1 == INFINITY) {
        impulse = mass2 * (1+c) * (vec_dot(v2, axis) - vec_dot(v1, axis));
    }
    else if (mass2 == INFINITY) {
        impulse = mass1 * (1+c) * (vec_dot(v2, axis) - vec_dot(v1, axis));
    }
    else {
        impulse = mass1 * mass2 / (mass1+mass2) * (1+c) * (vec_dot(v2, axis) - vec_dot(v1, axis));
    }
    body_add_impulse(body1, vec_multiply(impulse, axis));
    if (strcmp(body_get_info(body2), "disappearing platform") == 0) {
        body_remove(body2);
    }
    if (mass2 == 0) {
        body_set_velocity(body2, vec_multiply(-1 * BOOST, axis));
    }
    else {
        body_add_impulse(body1, vec_multiply(BOOST*mass1, axis));
        body_add_impulse(body2, vec_multiply(-1 * impulse, axis));
    }
}

void create_platform_collision(scene_t *scene, double elasticity, body_t *body1, body_t *body2) {
    force_aux_t *aux = force_aux_init(elasticity);
    list_t *bodies1 = list_init(2, NULL);
    // list_t *bodies2 = list_init(2, NULL);
    list_add(bodies1, body1);
    list_add(bodies1, body2);
    // list_add(bodies2, body1);
    // list_add(bodies2, body2);
    force_aux_set_bodies(aux, bodies1);
    create_collision(scene, body1, body2, (collision_handler_t)platform_collision, aux, (free_func_t)force_aux_free, platform_collided);
}

void powerup_collision(body_t *body1, body_t *body2, vector_t axis, void *aux) {
    if (strcmp(body_get_info(body1), "magnet") == 0 ||strcmp(body_get_info(body1), "immunity") == 0) {
        char *info = malloc(10*sizeof(char));
        strcpy(info, "collected");
        body_set_second_info(body1, info);
    }
    else if (strcmp(body_get_info(body2), "magnet") == 0||strcmp(body_get_info(body2), "immunity") == 0) {
        char *info = malloc(10*sizeof(char));
        strcpy(info, "collected");
        body_set_second_info(body2, info);
    }
}

void create_powerup_collision(scene_t *scene, double elasticity, body_t *body1, body_t *body2) {
    force_aux_t *aux = force_aux_init(elasticity);
    list_t *bodies1 = list_init(2, NULL);
    // list_t *bodies2 = list_init(2, NULL);
    list_add(bodies1, body1);
    list_add(bodies1, body2);
    // list_add(bodies2, body1);
    // list_add(bodies2, body2);
    force_aux_set_bodies(aux, bodies1);
    create_collision(scene, body1, body2, (collision_handler_t)powerup_collision, aux, (free_func_t)force_aux_free, collided);
}

void immunity_powerup_collision(body_t *body1, body_t *body2, vector_t axis, void *aux) {
    if (strcmp(body_get_info(body1), "enemy") == 0) {
        body_remove(body1);
    }
    else if (strcmp(body_get_info(body2), "enemy") == 0) {
        body_remove(body2);
    }
}

void create_immunity_collision(scene_t *scene, double elasticity, body_t *body1, body_t *body2) {
    force_aux_t *aux = force_aux_init(elasticity);
    list_t *bodies1 = list_init(2, NULL);
    list_add(bodies1, body1);
    list_add(bodies1, body2);
    force_aux_set_bodies(aux, bodies1);
    create_collision(scene, body1, body2, (collision_handler_t)immunity_powerup_collision, aux, (free_func_t)force_aux_free, collided);
}


void boost_powerup_collision(body_t *body1, body_t *body2, vector_t axis, void *aux) {
    force_aux_t *a = (force_aux_t *)aux;
    double c = force_aux_get_constant(a);
    double mass1 = body_get_mass(body1);
    double mass2 = body_get_mass(body2);
    vector_t v1 = body_get_velocity(body1);
    vector_t v2 = body_get_velocity(body2);
    double impulse;
    if (mass1 == INFINITY) {
        impulse = mass2 * (1+c) * (vec_dot(v2, axis) - vec_dot(v1, axis));
    }
    else if (mass2 == INFINITY) {
        impulse = mass1 * (1+c) * (vec_dot(v2, axis) - vec_dot(v1, axis));
    }
    else {
        impulse = mass1 * mass2 / (mass1+mass2) * (1+c) * (vec_dot(v2, axis) - vec_dot(v1, axis));
    }
    vector_t collision_axis = {.x = 0, .y = 1};
    if (mass1 == INFINITY) {
        char *info = malloc(9*sizeof(char));
        strcpy(info, "equipped");
        body_set_second_info(body1, info);
        body_add_impulse(body2, vec_multiply(impulse + BOOST_POWERUP*mass2, collision_axis));
    }
    else if (mass2 == INFINITY) {
        char *info = malloc(9*sizeof(char));
        strcpy(info, "equipped");
        body_set_second_info(body2, info);
        body_add_impulse(body1, vec_multiply(impulse + BOOST_POWERUP*mass1, collision_axis));
    }
}

void create_boost_collision(scene_t *scene, double elasticity, body_t *body1, body_t *body2) {
    force_aux_t *aux = force_aux_init(elasticity);
    list_t *bodies1 = list_init(2, NULL);
    list_add(bodies1, body1);
    list_add(bodies1, body2);
    force_aux_set_bodies(aux, bodies1);
    create_collision(scene, body1, body2, (collision_handler_t)boost_powerup_collision, aux, (free_func_t)force_aux_free, collided);
}

void star_collision(body_t *body1, body_t *body2, vector_t axis, void *aux) {
    char *info = malloc(10*sizeof(char));
    strcpy(info, "collected");

    if (strcmp(body_get_info(body1), "star") == 0) {
        body_set_second_info(body1, info);
    }
    else if (strcmp(body_get_info(body2), "star") == 0) {
        body_set_second_info(body2, info);
    }
}

void star_collided(void *a) {
    force_aux_t *aux = ((collision_package_t *)a)->aux;
    list_t *bodies = force_aux_get_bodies(aux);
    body_t *body1 = list_get(bodies, 0);
    body_t *body2 = list_get(bodies, 1);
    if (find_collision(body_get_shape(body1), body_get_shape(body2)).collided) {
        if (((collision_package_t *)a)->collided) {
            return;
        }
        ((collision_package_t *)a)->collided = true;
        vector_t axis = find_collision(body_get_shape(body1), body_get_shape(body2)).axis;
        ((collision_package_t *)a)->handler(body1, body2, axis, aux);
    }
}

void create_star_collision(scene_t *scene, double elasticity, body_t *body1, body_t *body2) {
    force_aux_t *aux = force_aux_init(elasticity);
    list_t *bodies1 = list_init(2, NULL);
    list_add(bodies1, body1);
    list_add(bodies1, body2);
    force_aux_set_bodies(aux, bodies1);
    create_collision(scene, body1, body2, (collision_handler_t)star_collision, aux, (free_func_t)force_aux_free, star_collided);
}
