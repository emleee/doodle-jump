#include <stddef.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "polygon.h"
#include "color.h"
#include "list.h"
#include <time.h>
#include <stdio.h>
#include "forces.h"
#include "collision.h"
#include "platforms.h"
#include "sdl_wrapper.h"
#include "game_sprites.h"
#include "SDL2/SDL_mixer.h"
#include "text.h"

const double WIDTH2 = 720.0;
const double HEIGHT2 = 960.0;

const double STARTING_PLATFORMS = 7; // get rid of this later?
const double MAX_PLATFORMS = 20;
const double PLATFORM_WIDTH2 = 60;

const vector_t START_VELOCITY = {.x = 0, .y = 300};
const double PLAYER_X_VELOCITY = 600;

const double SCORE_FACTOR = 20;

const rgb_color_t DOODLE_BODY_COLOR = {.r = 0.5, .g = 0.5, .b = 0.5};
const double DOODLE_MASS = 5.0;
const double MAX_JUMP = 250.0; // calculate this

const double G = -150.0;

// modify based on theresa's stuff
body_t *make_doodle(vector_t center, rgb_color_t color, char *info) {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {0, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {109, 0};   // magic numbers
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {109, 170};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {0, 170};
    list_add(shape, v);

    sprite_t *doodle_sprite = make_jump_right();
    body_t *doodle = body_init_with_sprite(shape, 10, color, info, free, doodle_sprite);
    body_set_centroid(doodle, center);

    return doodle;
}

body_t *make_background_body(vector_t center) {
    char *info = malloc(11*sizeof(char));
    strcpy(info, "background");

    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {-1, 1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {1, 1};   // magic numbers
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {-1, -1};
    list_add(shape, v);

    sprite_t *sprite = make_background();
    body_t *background = body_init_with_sprite(shape, 1, DOODLE_BODY_COLOR, info, free, sprite);
    body_set_centroid(background, center);
    return background;
}

scene_t *make_scene() {
    scene_t *scene = scene_init();

    // doodle
    char *doodle_info = malloc(7*sizeof(char));
    strcpy(doodle_info, "doodle");
    vector_t start = {.x = WIDTH2/2, .y = 0};

    body_t *doodle = make_doodle(start, DOODLE_BODY_COLOR, doodle_info);

    sprite_t *right_jump = body_get_sprite(doodle);
    sprite_t *left_jump = make_jump_left();
    scene_add_sprite(scene, right_jump);
    scene_add_sprite(scene, left_jump);

    body_set_velocity(doodle, START_VELOCITY);
    scene_add_body(scene, doodle);
    create_downward_gravity(scene, G, doodle);

    body_t *background1 = make_background_body((vector_t){.x = 0, .y = 960});
    body_t *background2 = make_background_body((vector_t){.x = 0, .y = 1920});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);

    // generates evenly spaced starting platforms, will change later
    vector_t scale = {.x = 0, .y = HEIGHT2 / STARTING_PLATFORMS};
    for(int i = 0; i < STARTING_PLATFORMS; i++) {
        vector_t center = vec_multiply(i+1, scale); //change once collisions are implemented
        center.x = (double)rand()/RAND_MAX * (WIDTH2 - PLATFORM_WIDTH2) + PLATFORM_WIDTH2/2;
        body_t *platform;
        if (i % 2 == 1) {
            char *info = malloc(24*sizeof(char));
            strcpy(info, "essential platform");
            platform = normal_platform(center, info);
        }
        else {
            char *info = malloc(22*sizeof(char));
            strcpy(info, "nonessential platform");
            platform = normal_platform(center, info);
        }
        scene_add_body(scene, platform);
        create_platform_collision(scene, 0, doodle, platform);
    }

    return scene;
}

bool in_screen(vector_t center, body_t *body) {
    list_t *points = body_get_shape(body);
    for (int i = 0; i < list_size(points); i++) {
        if (((vector_t *)list_get(points, i))->y > center.y - HEIGHT2/2) {
            return true;
        }
    }
    return false;
}

body_t *wrap(body_t *doodle) {
    if (body_get_centroid(doodle).x >= WIDTH2) { // >= width plus width of doodle /2
        vector_t shift = {.x = 0, .y = body_get_centroid(doodle).y};
        body_set_centroid(doodle, shift);
    }
    else if (body_get_centroid(doodle).x <= 0) { // <= 0 minus width of doodle /2
        vector_t shift = {.x = WIDTH2, .y = body_get_centroid(doodle).y};
        body_set_centroid(doodle, shift);
    }
    return doodle;
}

void more_platforms(scene_t *scene, vector_t center) {
    int num_platforms = 0;
    for (int i = 3; i < scene_bodies(scene); i++) {
        body_t *platform = scene_get_body(scene, i);
        char *info = body_get_info(platform);
        if (strstr(info, "platform") == NULL) {
            continue;
        }
        num_platforms++;
        if (strcmp("essential platform", info) == 0) {
            // essential platforms are generated one jump height apart
            double new_height = body_get_centroid(platform).y + MAX_JUMP;
            // only want to generate platforms that will be within one screen height above the current window
            if ((new_height > center.y + HEIGHT2/2) && (new_height < center.y + HEIGHT2/2 + HEIGHT2)) {
                strcat(info, " done");
                vector_t platform_center = {.x = (double)rand()/RAND_MAX * (WIDTH2 - PLATFORM_WIDTH2) + PLATFORM_WIDTH2/2, .y = new_height};
                char *new_info = malloc(24*sizeof(char));
                strcpy(new_info, "essential platform");
                body_t *new_platform = normal_platform(platform_center, new_info);
                scene_add_body(scene, new_platform);
                create_platform_collision(scene, 0, scene_get_body(scene, 0), new_platform);
            }
        }
    }
    for (int i = num_platforms; i < MAX_PLATFORMS; i++) {
        char *info = malloc(22*sizeof(char));
        strcpy(info, "nonessential platform");
        int random = rand() % 4;
        vector_t platform_center = {.x = (double)rand()/RAND_MAX * (WIDTH2 - PLATFORM_WIDTH2) + PLATFORM_WIDTH2/2, .y = (double)rand()/RAND_MAX * HEIGHT2 + center.y + HEIGHT2/2};
        if (random < 2) {
            body_t *new_platform = trick_platform(platform_center, info);
            scene_add_body(scene, new_platform);
            create_platform_collision(scene, 0, scene_get_body(scene, 0), new_platform);
        }
        else if (random == 2) {
            body_t *new_platform = sliding_platform(platform_center, info);
            scene_add_body(scene, new_platform);
            create_platform_collision(scene, 0, scene_get_body(scene, 0), new_platform);
        }
        else {
            body_t *new_platform = normal_platform(platform_center, info);
            scene_add_body(scene, new_platform);
            create_platform_collision(scene, 0, scene_get_body(scene, 0), new_platform);
        }
    }
}

void on_key(char key, key_event_type_t type, double held_time, void *scene) {
    body_t *player = scene_get_body((scene_t *)scene, 0);
    vector_t body_velocity = body_get_velocity(player);
    // double mass = body_get_mass(player);
    if (type == KEY_PRESSED) {
        switch (key) {
            case RIGHT_ARROW:
                body_set_rotation(player, 0);
                if (body_get_sprite(player) == scene_get_sprite(scene, 1)) {
                    face_right(player, scene_get_sprite(scene, 0));
                }
                body_velocity.x = PLAYER_X_VELOCITY;
                body_set_velocity(player, body_velocity);
                if (loadMedia()) {
                    Mix_Chunk *jump = (Mix_Chunk *) get_jump();
                    Mix_PlayChannel( -1, jump, 0 );
                }
                break;
            case LEFT_ARROW:
                body_set_rotation(player, M_PI);
                if (body_get_sprite(player) == scene_get_sprite(scene, 0)) {
                    face_left(player, scene_get_sprite(scene, 1));
                }
                body_velocity.x = -1 * PLAYER_X_VELOCITY;
                body_set_velocity(player, body_velocity);
                if (loadMedia()) {
                    Mix_Chunk *jump = (Mix_Chunk *) get_jump();
                    Mix_PlayChannel( -1, jump, 0 );
                }
                break;
        }
    }
    else {
        body_velocity.x = 0;
        body_set_velocity(player, body_velocity);
    }
}

double calculate_score(vector_t center) {
    // find doodle center height
    double height = center.y;

    // calculate score based on certain divisor or smth
    double score = height / SCORE_FACTOR;

    return score;

    // probably need to account for just the Highest doodle reaches? so score doesn't fluctuate
    // randomly as doodle falls, maybe do this somewhere else tho
    // like per tick, check if score > current score, if so replace otherwise don't
}

int main() {
    vector_t start_min = {.x = 0, .y = 0};
    vector_t start_max = {.x = WIDTH2, .y = HEIGHT2};
    sdl_init(start_min, start_max);
    srand(time(0));

    sdl_on_key(on_key);
    scene_t *scene = make_scene();
    body_t *doodle = scene_get_body(scene, 0);

    vector_t center = {.x = WIDTH2/2, HEIGHT2/2};

    rgb_color_t color = {.r = 0, .g = 0, .b = 0};
    vector_t *point = malloc(sizeof(vector_t));
    point->x = 250; // remove magic numbers
    point->y = 10;
    text_t *text = text_create("Doodle Jump: Fairy Tail", color, 22, point, 200, 25);
    scene_add_text(scene, text);

    vector_t *scoring = malloc(sizeof(vector_t));
    scoring->x = 10;
    scoring->y = 10;

    char *score = malloc(100*sizeof(char));

    // char score[100];

    // score = "High Score: ";
    // strcat(score, snprintf(score, 50, "%d", calculate_score(scene)));
    // FILE *file = fopen("highscores.txt", "w");
    // fprintf(file, score);


    // score = calculate_score(scene);
    // text_t *score = text_create(score, color, 20, scoring, 20, 20);

    while (!sdl_is_done(scene)) {
        // calculate and display score
        if (scene_textboxes(scene) > 1) {
            scene_remove_text(scene, scene_get_text(scene, scene_textboxes(scene) - 1));
        }
        // printf("\n%i\n", scene_textboxes(scene));
        strcpy(score, "High Score: ");
        // printf("\n%f\n", calculate_score(center));
        double curr = calculate_score(center);

        // strcat(sprintf(score, "%f", curr), '\n');
        sprintf(score, "%.1", curr);
        // printf("\n%s\n", score);
        // strcat(score, '\n');
        text_t *scorebox = text_create(score, color, 20, scoring, 100, 20);
        scene_add_text(scene, scorebox);

        double dt = time_since_last_tick();

        if (!in_screen(center, doodle)) {
            // PLAYER LOSES, REPLACE BREAK WITH ACTUAL CODE
            break;
        }

        // freeing platforms, etc that are out of the screen
        for(int i = 1; i < scene_bodies(scene); i++) {
            if (!in_screen(center, scene_get_body(scene, i))) {
                scene_remove_body(scene, i);
            }
        }

        // shfiting the viewing window if the doodle goes higher than the center
        if (body_get_centroid(doodle).y > center.y) {
            // generates more platforms
            more_platforms(scene, center);
            center.y = body_get_centroid(doodle).y;
            sdl_set_center(center);
        }

        wrap(doodle);
        // printf("body.x %f\nbody.y %f\nsprite.x %f\nsprite.y %f\n", body_get_centroid(scene_get_body(scene, 0)).x, body_get_centroid(scene_get_body(scene, 0)).y, sprite_get_center(body_get_sprite(scene_get_body(scene, 0))).x, sprite_get_center(body_get_sprite(scene_get_body(scene, 0))).y);
        scene_tick(scene, dt);
        sdl_render_scene(scene);
    }
    FILE *file = fopen("highscores.txt", "w");
    fprintf(file, score);
    fclose(file);
    free(score);
    scene_free(scene);
}
