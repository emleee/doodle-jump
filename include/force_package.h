#ifndef __FORCE_PACKAGE_H__
#define __FORCE_PACKAGE_H__

#include "scene.h"

/**
 * Holds a force creator, force_aux, and a force freer.
 */
typedef struct force_package force_package_t;

/**
 * Allocates memory for a force_package_t containing the given components.
 * 
 * @param forcer a force_creator_t
 * @param aux an auxilary variable, usually is a force_aux_t
 * @param bodies a pointer to a list of bodies
 * @param freer the aux's freer
 * @return a newly allocated force_package_t
 */
force_package_t *force_package_init(force_creator_t forcer, void *aux, list_t *bodies, free_func_t freer);

/**
 * Checks if a force_package is applied to the given body.
 * 
 * @param force_package a force_package_t
 * @param body the pointer to the body being matched
 * @return a boolean representing if the body is matched with
 */
bool package_contains(force_package_t *force_package, body_t *body);

/**
 * Returns the force_creator_t associated with a force_package
 * 
 * @param force_package a force_package_t
 * @return the package's force_creator_t
 */
force_creator_t get_force_creator(force_package_t *force_package);

/**
 * Returns the aux associated with a force_package_t
 * 
 * @param force_package a force_package_t
 * @return the aux
 */
void *get_aux(force_package_t *force_package);

/**
 * Returns the list of bodies associated with a force_package_t
 * 
 * @param force_package a force_package_t
 * @return the list
 */
list_t *get_bodies(force_package_t *force_package);

/**
 * Returns the constant associated with a force_package_t
 * 
 * @param force_package a force_package_t
 * @return the constant
 */
double package_get_constant(force_package_t *force_package);

/**
 * Returns the freer for the aux associated with a force_package_t
 * 
 * @param force_package a force_package_t
 * @return the freer
 */
free_func_t get_force_freer(force_package_t *force_package);

/**
 * Frees a force_package_t and all of its components.
 * 
 * @param force_package a force_pacakge_t
 */
void force_package_free(force_package_t *force_package);

#endif // #ifndef __SCENE_H__