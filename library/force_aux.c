// #include "list.h"
// #include "body.h"
// #include <stdlib.h>
#include <assert.h>
// #include <stdio.h>
// #include <stdbool.h>
#include "force_aux.h"

const int DEFAULT_BODIES = 2;

typedef struct force_aux {
    list_t *list;
    double constant;
} force_aux_t;

force_aux_t *force_aux_init(double constant) {
    force_aux_t *force_aux = malloc(sizeof(force_aux_t));
    assert(force_aux != NULL);
    force_aux->constant = constant;
    force_aux->list = NULL;
    return force_aux;
}

void force_aux_add_body(force_aux_t *force_aux, body_t *body) {
    list_add(force_aux->list, body);
}

void force_aux_set_bodies(force_aux_t *force_aux, list_t *bodies) {
    if (force_aux->list != NULL) {
        list_free(force_aux->list);
    }
    force_aux->list = bodies;
}

list_t *force_aux_get_bodies(force_aux_t *force_aux) {
    return force_aux->list;
}

bool force_aux_contains(force_aux_t *force_aux, body_t *body) {
    for (size_t i = 0; i < list_size(force_aux->list); i++) {
        if (list_get(force_aux->list, i) == body) {
            return true;
        }
    }
    return false;
}

double force_aux_get_constant(force_aux_t *force_aux) {
    return force_aux->constant;
}

void force_aux_free(force_aux_t *force_aux) {
    list_free(force_aux->list);
    free(force_aux);
}