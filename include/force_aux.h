#ifndef __FORCE_AUX_H__
#define __FORCE_AUX_H__

#include "body.h"

/**
 * A thing that will hold the constants and bodies that need
 * to be passed into a force function.
 */
typedef struct force_aux force_aux_t;

/**
 * Allocates memory.
 */
force_aux_t *force_aux_init(double constant);

/**
 * Adds a body to the list.
 */
void force_aux_add_body(force_aux_t *force_aux, body_t *body);

/**
 * Sets the list to a list of bodies.
 */
void force_aux_set_bodies(force_aux_t *force_aux, list_t *bodies);

/**
 * Checks if list contains a body.
 */
bool force_aux_contains(force_aux_t *force_aux, body_t *body);

/**
 * Returns the list of bodies.
 */
list_t *force_aux_get_bodies(force_aux_t *force_aux);

double force_aux_get_constant(force_aux_t *force_aux);

/**
 * Frees force_aux
 */
void force_aux_free(force_aux_t *force_aux);

bool force_aux_get_collided(force_aux_t *force_aux);

void force_aux_set_collided(force_aux_t *force_aux, bool collided);

#endif // #ifndef __FORCE_AUX_H__