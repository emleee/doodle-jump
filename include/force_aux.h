#ifndef __FORCE_AUX_H__
#define __FORCE_AUX_H__

#include "body.h"

/**
 * A thing that will hold the constants and bodies that need
 * to be passed into a force function.
 */
typedef struct force_aux force_aux_t;

/**
 * Allocates memory for a new force_aux_t.
 * 
 * @param constant a constant to be stored in a force_aux
 * @return a newly allocated force_aux_t
 */
force_aux_t *force_aux_init(double constant);

/**
 * Adds a body to the list in a force_aux.
 * 
 * @param force_aux a pointer to a force_aux_t
 * @param body a pointer to the body being added
 */
void force_aux_add_body(force_aux_t *force_aux, body_t *body);

/**
 * Sets the list of bodies in a force aux to a different list
 * 
 * @param force_aux a pointer to a force_aux_t
 * @param bodies a pointer to a list of bodies
 */
void force_aux_set_bodies(force_aux_t *force_aux, list_t *bodies);

/**
 * Checks if the force_aux's list contains a specific.
 * 
 * @param force_aux a pointer to a force_aux_t
 * @param body a pointer to the body being matched
 * @return a boolean representing whether the body is found
 */
bool force_aux_contains(force_aux_t *force_aux, body_t *body);

/**
 * Returns the list of bodies associated wiht a force_aux.
 * 
 * @param force_aux the pointer to a force_aux_t
 * @return a pointer to the force_aux's list of bodies
 */
list_t *force_aux_get_bodies(force_aux_t *force_aux);

/**
 * Returns the constant associated with a force_aux_t.
 * 
 * @param force_aux the pointer to a force_aux_t
 * @return a double
 */
double force_aux_get_constant(force_aux_t *force_aux);

/**
 * Frees a force_aux and all of its components
 * 
 * @param force_aux a pointer to the force_aux_t
 */
void force_aux_free(force_aux_t *force_aux);

/**
 * Returns the value of the collided component of a force_aux
 * 
 * @param force_aux a pointer to a force_aux_t
 * @return the collided boolean
 */
bool force_aux_get_collided(force_aux_t *force_aux);

/**
 * Sets the collided value of a force_aux to something
 * 
 * @param force_aux a pointer to a force_aux_t
 * @param collided a boolean being put into force_aux
 */
void force_aux_set_collided(force_aux_t *force_aux, bool collided);

#endif // #ifndef __FORCE_AUX_H__