#ifndef __STAR_H__
#define __STAR_H__

#include "list.h"
#include "vector.h"

/**
 * A list of vectors that double as points that have a color
 * - Declares a "struct star" type
 * - Makes "star_t" an alias for "struct star"
 *
 */
typedef struct star star_t;

/**
 * Allocates memory for a new star with space for the given number of elements.
 * The star is initially empty.
 * Asserts that the required memory was allocated.
 * Random colors added
 *
 * @param initial_size the number of star elements to allocate space for
 * @return a pointer to the newly allocated star
 */
star_t *star_init(size_t initial_size);

/**
 * Computes points of star with given center, radius, and number of points.
 *
 * @param center the coordinates of the centroid of the star
 * @param radius the radius of the star
 * @param num_points the number of points in the star
 * @returns the star that was made
 */
star_t *make_star (vector_t center, int num_points, double radius);

/**
 * Changes the color of the given star to the given color values.
 *
 * @param star the star that's getting its colors changed
 * @param r value to change the r to
 * @param g value to change the g to
 * @param b value to change the b to
 */
void change_color(star_t *star, float r, float g, float b);

/**
 * Returns the value of r of the given star.
 *
 * @param star the star we're getting the color from
 * @returns the value of r
 */
float get_r(star_t *star);

/**
 * Returns the list of points for the star.
 *
 * @param star the star whose points are being returned
 * @returns the vec_list of points
 */
list_t *get_points(star_t *star);

/**
 * Returns the value of g of the given star.
 *
 * @param star the star we're getting the color from
 * @returns the value of g
 */
float get_g(star_t *star);

/**
 * Returns the value of b of the given star.
 *
 * @param star the star we're getting the color from
 * @returns the value of b
 */
float get_b(star_t *star);

/**
 * Returns the velocity of the given star.
 *
 * @param star the star we're getting the velocity of
 * @return the velocity vector
*/
vector_t *get_velocity(star_t *star);

/**
 * Returns the elasticity of the given star.
 *
 * @param star the star we're getting the elasticity of
 * @return the elasticity
*/
int get_elasticity(star_t *star);

/**
 * Uses kinematic equation to update the velocity.
 *
 * @param star the star whose velocity we want to update
 * @returns the new velocity
 */
vector_t *update_velocity(star_t *star, double dt);

/**
 * Frees the memory allocated to the star.
 *
 * @param star the star that's being freed
 */
void star_free(star_t *star);

#endif // #ifndef __STAR_H__