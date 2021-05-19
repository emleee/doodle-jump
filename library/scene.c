#include "scene.h"
#include "body.h"
#include "force_aux.h"
#include "force_package.h"
#include <stdlib.h>
#include <assert.h>
#include "sdl_wrapper.h"

typedef struct scene {
    list_t *bodies;
    list_t *forces;
    list_t *text;
} scene_t;

scene_t *scene_init(void) {
    scene_t *scene = malloc(sizeof(scene_t));
    assert(scene != NULL);
    scene->bodies = list_init(100, (free_func_t)body_free);
    scene->forces = list_init(100, (free_func_t)force_package_free);
    scene->bodies = list_init(100, (free_func_t)free_text);
    return scene;
}

void scene_free(scene_t *scene) {
    list_free(scene->bodies);
    list_free(scene->text);
    free(scene);
}

size_t scene_bodies(scene_t *scene) {
    return list_size(scene->bodies);
}

body_t *scene_get_body(scene_t *scene, size_t index) {
    assert(index < scene_bodies(scene));
    return list_get(scene->bodies, index);
}

void scene_add_body(scene_t *scene, body_t *body) {
    list_add(scene->bodies, body);
}

void scene_remove_body(scene_t *scene, size_t index) {
    assert(index < scene_bodies(scene));
    body_remove(scene_get_body(scene, index));
}

list_t *scene_get_forces(scene_t *scene) {
    return scene->forces;
}

void scene_add_force_creator(scene_t *scene, force_creator_t forcer, void *aux, free_func_t freer) {
    scene_add_bodies_force_creator(scene, forcer, aux, force_aux_get_bodies(aux), freer);
}

void scene_add_bodies_force_creator(scene_t *scene, force_creator_t forcer, void *aux, list_t *bodies, free_func_t freer) {
    force_package_t *force = force_package_init(forcer, aux, bodies, freer);
    list_add(scene->forces, force);
}

void scene_add_text(scene_t *scene, text_t *text) {
    list_add(scene->text, text);
}

void scene_remove_text(scene_t *scene, text_t *text) {
    size_t index = -1;
    for (size_t i = 0; i < list_size(scene->text); i++) {
        if (list_get(scene->text, i) == text) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        text_t *removed = list_remove(scene->text, index);
        free_text(removed);
    }
}

text_t *scene_get_text(scene_t *scene, size_t index) {
    return list_get(scene->text, index);
}

text_t *scene_textboxes(scene_t *scene) {
    return list_size(scene->text);
}

void scene_tick(scene_t *scene, double dt) {
    for (size_t i = 0; i < list_size(scene->forces); i++) {
        force_package_t *force = list_get(scene->forces, i);
        force_aux_t *aux = get_aux(force);
        get_force_creator(force)(aux);
    }

    for (size_t i = 0; i < scene_bodies(scene); i++) {
        body_tick(scene_get_body(scene, i), dt);
    }

    for (size_t i = 0; i < scene_bodies(scene); i++) {
        if (body_is_removed(scene_get_body(scene, i))) {
            // check if it has force creators on it
            for (size_t j = 0; j < list_size(scene->forces); j++) {
                if (package_contains(list_get(scene->forces, j), scene_get_body(scene, i))) {
                    force_package_t *package = list_remove(scene->forces, j);
                    force_package_free(package);
                    j--;
                }
            }
            body_t *body = list_remove(scene->bodies, i);
            body_free(body);
            i--;
        }
    }

    // add a thing about displaying the text
}






