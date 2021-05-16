#ifndef __FORCE_PACKAGE_H__
#define __FORCE_PACKAGE_H__

#include "scene.h"

/**
 * Holds a force creator, force_aux, and a force freer.
 */
typedef struct force_package force_package_t;

force_package_t *force_package_init(force_creator_t forcer, void *aux, list_t *bodies, free_func_t freer);

bool package_contains(force_package_t *force_package, body_t *body);

force_creator_t get_force_creator(force_package_t *force_package);

void *get_aux(force_package_t *force_package);

list_t *get_bodies(force_package_t *force_package);

double package_get_constant(force_package_t *force_package);

free_func_t get_force_freer(force_package_t *force_package);

void force_package_free(force_package_t *force_package);

#endif // #ifndef __SCENE_H__