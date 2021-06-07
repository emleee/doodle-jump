#include <assert.h>
#include "force_aux.h"
#include "force_package.h"

typedef struct force_package {
    force_creator_t forcer;
    void *aux;
    double constant;
    list_t *bodies;
    free_func_t freer;
} force_package_t;

force_package_t *force_package_init(force_creator_t forcer, void *aux, list_t *bodies, free_func_t freer) {
    force_package_t *force = malloc(sizeof(force_package_t));
    assert(force != NULL);
    force->forcer = forcer;
    force->aux = aux;
    force->constant = force_aux_get_constant(aux);
    force->bodies = bodies;
    force->freer = freer;
    return force;
}

bool package_contains(force_package_t *force_package, body_t *body) {
    for (size_t i = 0; i < list_size(force_package->bodies); i++) {
        if (list_get(force_package->bodies, i) == body) {
            return true;
        }
    }
    return false;
}

double package_get_constant(force_package_t *force_package) {
    return force_package->constant;
}

force_creator_t get_force_creator(force_package_t *force_package) {
    return force_package->forcer;
}

void *get_aux(force_package_t *force_package) {
    return force_package->aux;
}

free_func_t get_force_freer(force_package_t *force_package) {
    return (free_func_t)force_package->freer;
}

list_t *get_bodies(force_package_t *force_package) {
    return force_package->bodies;
}

void force_package_free(force_package_t *force_package) {
    if (force_package->freer != NULL) {
        force_package->freer(force_package->aux);
    }
    list_free(force_package->bodies);
    free(force_package);
}
