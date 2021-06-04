#ifndef __SCENE_H__
#define __SCENE_H__

#include "body.h"
#include "text.h"

/**
 * A collection of bodies and force creators.
 * The scene automatically resizes to store
 * arbitrarily many bodies and force creators.
 */
typedef struct scene scene_t;

/**
 * A function which adds some forces or impulses to bodies,
 * e.g. from collisions, gravity, or spring forces.
 * Takes in an auxiliary value that can store parameters or state.
 */
typedef void (*force_creator_t)(void *aux);

/**
 * Allocates memory for an empty scene.
 * Makes a reasonable guess of the number of bodies to allocate space for.
 * Asserts that the required memory is successfully allocated.
 *
 * @return the new scene
 */
scene_t *scene_init(void);

/**
 * Allocates memory for a scene with the given parameters.
 *
 * @param info information to associate with the scene
 * @param info_freer if non-NULL, a function call on the info to free it
 * @return a pointer to the newly allocated scene
 */
scene_t *scene_init_with_info(void *info, free_func_t info_freer);

/**
 * Retuns the sprite of a scene at the given position.
 * 
 * @param scene a pointer to the scene
 * @param index the index of the sprite
 */
sprite_t *scene_get_sprite(scene_t *scene, size_t index);

/**
 * Adds a sprite to a scene
 * 
 * @param scene the pointer to a scene
 * @param sprite the sprite to be added
 */
void scene_add_sprite(scene_t *scene, sprite_t *sprite);

/**
 * Releases memory allocated for a given scene
 * and all the bodies and force creators it contains.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void scene_free(scene_t *scene);

/**
 * Gets the number of bodies in a given scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @return the number of bodies added with scene_add_body()
 */
size_t scene_bodies(scene_t *scene);

/**
 * Gets the body at a given index in a scene.
 * Asserts that the index is valid.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param index the index of the body in the scene (starting at 0)
 * @return a pointer to the body at the given index
 */
body_t *scene_get_body(scene_t *scene, size_t index);

/**
 * Adds a body to a scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param body a pointer to the body to add to the scene
 */
void scene_add_body(scene_t *scene, body_t *body);

/**
 * Gets the information associated with a scene.
 *
 * @param scene a pointer to a scene returned from scene_init_with_info()
 * @return the info passed to scene_init_with_info()
 */
void *scene_get_info(scene_t *scene);

/**
 * Gets the additional information associated with a scene.
 *
 * @param scene a pointer to a scene returned from scene_init_with_info()
 * @return the additional info passed to scene_init_with_info() or changed by scene_set_next_info()
 */
void *scene_get_next_info(scene_t *scene);

/**
 * Sets the additional information associated with a scene.
 *
 * @param scene a pointer to a scene returned from scene_init_with_info()
 */
void scene_set_next_info(scene_t *scene, void *next_info);

/**
 * @deprecated Use body_remove() instead
 *
 * Removes and frees the body at a given index from a scene.
 * Asserts that the index is valid.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param index the index of the body in the scene (starting at 0)
 */
void scene_remove_body(scene_t *scene, size_t index);

/**
 * Returns the list of forces being applied to the bodies in a scene.
 * 
 * @param scene the pointer to the scene
 * @return the list of force packages
 */
list_t *scene_get_forces(scene_t *scene);

/**
 * @deprecated Use scene_add_bodies_force_creator() instead
 * so the scene knows which bodies the force creator depends on
 */
void scene_add_force_creator(
    scene_t *scene,
    force_creator_t forcer,
    void *aux,
    free_func_t freer
);

/**
 * Adds a force creator to a scene,
 * to be invoked every time scene_tick() is called.
 * The auxiliary value is passed to the force creator each time it is called.
 * The force creator is registered with a list of bodies it applies to,
 * so it can be removed when any one of the bodies is removed.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param forcer a force creator function
 * @param aux an auxiliary value to pass to forcer when it is called
 * @param bodies the list of bodies affected by the force creator.
 *   The force creator will be removed if any of these bodies are removed.
 *   This list does not own the bodies, so its freer should be NULL.
 * @param freer if non-NULL, a function to call in order to free aux
 */
void scene_add_bodies_force_creator(
    scene_t *scene,
    force_creator_t forcer,
    void *aux,
    list_t *bodies,
    free_func_t freer
);

/**
 * Adds a textbox to a scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param text the textbox to add
 */
void scene_add_text(scene_t *scene, text_t *text);

/**
 * Removes a textbox from a scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param text the textbox to remove
 */
void scene_remove_text(scene_t *scene, text_t *text);

/**
 * Gets the textbox at a certain index from the scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param index the index of the textbox to get
 * @return a textbox from the scene
 */
text_t *scene_get_text(scene_t *scene, size_t index);

/**
 * Gets the number of textboxes in a given scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @return the number of textboxes
 */
size_t scene_textboxes(scene_t *scene);

/**
 * Increases the counter of stars collected by one.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void scene_increase_stars(scene_t *scene);

/**
 * Gets the number of stars in a given scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @return the number of stars
 */
int scene_stars(scene_t *scene);

/**
 * Executes a tick of a given scene over a small time interval.
 * This requires executing all the force creators
 * and then ticking each body (see body_tick()).
 * If any bodies are marked for removal, they should be removed from the scene
 * and freed, along with any force creators acting on them.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param dt the time elapsed since the last tick, in seconds
 */
void scene_tick(scene_t *scene, double dt);

#endif // #ifndef __SCENE_H__
