#include "powerups.h"
#include "game.h"
#include "game_sprites.h"

const rgb_color_t BOOST_COLOR = {.r = 106.0/255, .g = 77.0/255, .b = 255.0/255};
const rgb_color_t IMMUNITY_COLOR = {.r = 0.54, .g = 0.54, .b = 0.54};
const rgb_color_t MAGNET_COLOR = {.r = 1, .g = 1, .b = 0.2};
const double RADIUS = 176.0; //152.0
const double RADIUS2 = 20.0; //152.0
const double POWERUP_OFFSET = 50.0;

const int BOOST_IDX = 1;
const int IMMUNITY_IDX = 2;
const int MAGNET_IDX = 3;

const int NEWTONIAN_GRAVITY = -150;
const int MAGNET_GRAVITY = 5000000;

bool is_powerup(body_t *body) {
    if (strcmp(body_get_info(body), "magnet") == 0 || strcmp(body_get_info(body), "immunity") == 0 || strcmp(body_get_info(body), "boost") == 0 ) {
        return true;
    }
    return false;
}

// move this to platforms.c so Lucy can use it for create_star!
vector_t *platform_center(scene_t *scene) {
    int random = 0;
    char *info = body_get_info(scene_get_body(scene, random));
    bool conflict = false;
    int counter = 0;
    vector_t *center = malloc(sizeof(vector_t));
    do {
        if (counter == scene_bodies(scene)) {
            return NULL;
        }

        while (strcmp("normal platform", info) != 0) {
            random = rand() % scene_bodies(scene);
            info = body_get_info(scene_get_body(scene, random));
        }

        // check if platform already has a star on it
        body_t *body2 = scene_get_body(scene, random);
        vector_t centroid = body_get_centroid(body2);
        centroid.y += 40;

        for (size_t j = 0; j < scene_bodies(scene); j++) {
            body_t *body1 = scene_get_body(scene, j);
            if ((strcmp(body_get_info(body1), "star") == 0 || is_powerup(body1)) && random != j && vec_isclose(body_get_centroid(body1), centroid)) {
                conflict = true;
            }
        }
        counter++;
    }
    while (conflict);

    body_t *platform = scene_get_body(scene, random);
    center->x = body_get_centroid(platform).x;
    center->y = body_get_centroid(platform).y + POWERUP_OFFSET; // magic number for offset
    if ((*center).y <= body_get_centroid(scene_get_body(scene,0)).y) {
        return NULL;
    }
    return center;
}

body_t *make_powerup(scene_t *scene, bool enemy_present) {
    int num_powerups = 0;
    for (int i = 0; i < scene_bodies(scene); i++) {
        body_t *body = scene_get_body(scene, i);
        if (is_powerup(body)) {
            num_powerups++;
        }
    }
    int idx = -1;
    if (num_powerups == 0) {
        vector_t *center = platform_center(scene);
        if (center == NULL) {
            return NULL;
        }

        if (enemy_present) {
            idx = (rand() % (3 - 2 + 1)) + 2;
        }
        else {
            idx = (rand() % (3 - 1 + 1)) + 1;
        }

        if (idx == BOOST_IDX) {
            return make_boost(scene, *center);
        }
        else if (idx == IMMUNITY_IDX) {
            return make_immunity(scene, *center, false);
        }
        else if (idx == MAGNET_IDX) {
            return make_magnet(scene, *center, false);
        }        
        // return make_magnet(scene, *center, false);
        free(center);
    }
    return NULL;
}

body_t *make_boost(scene_t *scene, vector_t center){
    printf("made boost\n");
    list_t *shape = make_rectangle(VEC_ZERO, 1075/13, 843/13);
    char *info = malloc(sizeof(char)*6);
    strcpy(info, "boost");
    body_t *boost = body_init_with_info(shape, INFINITY, BOOST_COLOR, info, free);
    sprite_t *sprite = create_sprite("PNGs/Boost.png", 1075/13, 843/13);
    body_set_sprite(boost, sprite);
    body_set_centroid(boost, center);

    scene_add_body(scene, boost);
    if (strcmp(scene_get_info(scene), "game") == 0) {
        create_boost_collision(scene, 0, scene_get_body(scene, 0), boost);
    }
    return boost;
}

body_t *make_immunity(scene_t *scene, vector_t center, bool collected) {
    list_t *shape = NULL;
    sprite_t *sprite = NULL;
    if (collected) {
        shape = make_circle(RADIUS);
        sprite = create_sprite("PNGs/Immunity.png", 1405/8, 1398/8);
    }
    else {
        shape = make_circle(RADIUS2);
        sprite = create_sprite("PNGs/Immunity.png", 2*RADIUS2, 2*RADIUS2);
    }
    char *info = malloc(sizeof(char)*9);
    strcpy(info, "immunity");
    body_t *immunity = body_init_with_info(shape, 10, IMMUNITY_COLOR, info, free);
    if (sprite != NULL) {
        body_set_sprite(immunity, sprite);
    }
    body_set_centroid(immunity, center);
    scene_add_body(scene, immunity);
    body_t *doodle = scene_get_body(scene, 0);
    if (!collected && strcmp(scene_get_info(scene), "game") == 0) {
        create_powerup_collision(scene, 0, doodle, immunity);
    }
    return immunity;
}

body_t *make_magnet(scene_t *scene, vector_t center, bool collected) {
    body_t *doodle = scene_get_body(scene, 0);
    list_t *shape = list_init(4, free); //make rectangle
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {0, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {96, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {96, 148};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {0, 148};
    list_add(shape, v);

    char *info = malloc(sizeof(char)*7);
    strcpy(info, "magnet");
    body_t *magnet = body_init_with_info(shape, INFINITY, MAGNET_COLOR, info, free);
    sprite_t *sprite = create_sprite("PNGs/Magnet.png", 748/21, 845/21);
    body_set_sprite(magnet, sprite);
    body_set_centroid(magnet, center);
    scene_add_body(scene, magnet);
    if (!collected && strcmp(scene_get_info(scene), "game") == 0) {
        create_powerup_collision(scene, 0, doodle, magnet);
    }
    return magnet;
}

void immunity_powerup(scene_t *scene, int *powerup_timer) {
    body_t *doodle = scene_get_body(scene, 0);
    size_t immunity_idx = -1;
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        body_t *body = scene_get_body(scene, i);
        if (strcmp(body_get_info(body), "immunity") == 0 && body_get_second_info(scene_get_body(scene, i)) != NULL && strcmp(body_get_second_info(scene_get_body(scene, i)), "collected") == 0) {
            immunity_idx = i;
        }
        if (strcmp(body_get_info(body), "immunity") == 0 && body_get_second_info(scene_get_body(scene, i)) != NULL && strcmp(body_get_second_info(scene_get_body(scene, i)), "equipped") == 0) {
            body_set_centroid(body, body_get_centroid(doodle));
        }
    }
    //move this to inner for loop
    if (immunity_idx != -1) {
        body_t *immunity = scene_get_body(scene, immunity_idx);
        scene_remove_body(scene, immunity_idx);
        immunity = make_immunity(scene, body_get_centroid(doodle), true);
        char *info = malloc(sizeof(char)*9);
        strcpy(info, "equipped");
        body_set_second_info(immunity, info);
        *powerup_timer = 0;
        body_set_mass(immunity, 100);
        for (size_t j = 0; j < scene_bodies(scene); j++) {
            body_t *body = scene_get_body(scene, j);
            if (strcmp(body_get_info(body), "enemy") == 0) {
                create_immunity_collision(scene, 0, immunity, body);
            }
        }
    }
}

void magnet_powerup(scene_t *scene, int *powerup_timer) {
    body_t *doodle = scene_get_body(scene, 0);
    size_t magnet_idx = -1;
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        body_t *body = scene_get_body(scene, i);
        if (strcmp(body_get_info(body), "magnet") == 0 && body_get_second_info(scene_get_body(scene, i)) != NULL && strcmp(body_get_second_info(scene_get_body(scene, i)), "collected") == 0) {
            magnet_idx = i;
        }
        if (strcmp(body_get_info(body), "magnet") == 0 && body_get_second_info(scene_get_body(scene, i)) != NULL && strcmp(body_get_second_info(scene_get_body(scene, i)), "equipped") == 0) {
            vector_t hand = find_hand(scene, doodle, body);
            body_set_centroid(body, hand);
        }
    }
    if (magnet_idx != -1) {
        body_t *magnet = scene_get_body(scene, magnet_idx);
        scene_remove_body(scene, magnet_idx);
        magnet = make_magnet(scene, body_get_centroid(doodle), true);
        char *info = malloc(sizeof(char)*9);
        strcpy(info, "equipped");
        body_set_second_info(magnet, info);
        *powerup_timer = 0;
        body_set_mass(magnet, 100);
        for (size_t j = 0; j < scene_bodies(scene); j++) {
            body_t *body = scene_get_body(scene, j);
            if (strcmp(body_get_info(body), "star") == 0 && in_screen(body_get_centroid(body), body)) {
                vector_t velocity = {.x = 1, .y = 1};
                body_set_velocity(body, velocity);
                create_magnetic_force(scene, MAGNET_GRAVITY, magnet, body);
            }
        }
    }
}