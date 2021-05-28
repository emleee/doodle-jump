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
#include "test_util.h"
#include "preferences.h"
#include "powerups.h"
#include "star.h"
#include "game.h"

const double GAME_WIDTH = 720.0;
const double GAME_HEIGHT = 960.0;
const double GAME_DOODLE_WIDTH = 96.0;
const double GAME_DOODLE_HEIGHT = 148.0;
const int NUM_POINTS = 50;
const double GAME_BUTTON_X_RADIUS = 125;
const double GAME_BUTTON_Y_RADIUS = 75;

const double MAX_PLATFORMS = 12;
const double PLATFORM_WIDTH2 = 146;
const double PLATFORM_HEIGHT2 = 35;

const vector_t GAME_START_VELOCITY = {.x = 0, .y = 300};
// const double PLAYER_X_VELOCITY = 600;

const double SCORE_FACTOR = 20;

const rgb_color_t GAME_DOODLE_COLOR = {.r = 176.0/255, .g = 128.0/255, .b = 124.0/255};
const rgb_color_t BUTTON_COLOR = {.r = 255.0/255, .g = 255.0/255, .b = 255.0/255};
const double DOODLE_MASS = 5.0;
const double BUTTON_MASS = INFINITY;
const double GAME_MAX_JUMP = 295.0;

const double GAME_G = -150.0;

const double BUTTON_OFFSET = 100;

body_t *make_enemy(vector_t center) {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {0, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {GAME_DOODLE_HEIGHT, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {GAME_DOODLE_HEIGHT, GAME_DOODLE_WIDTH};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {0, GAME_DOODLE_WIDTH};
    list_add(shape, v);

    char *info = malloc(sizeof(char)*6);
    strcpy(info, "enemy");
    body_t *doodle = body_init_with_info(shape, 10, GAME_DOODLE_COLOR, info, free);
    body_set_centroid(doodle, center);

    return doodle;
}

body_t *make_button(vector_t center) {
    // body shape
    list_t *points = list_init(NUM_POINTS, free);
    for (int i = 0; i < (NUM_POINTS); i++) {
        vector_t *pt = malloc(sizeof(vector_t));
        pt->x = GAME_BUTTON_X_RADIUS * cos(2 * M_PI * i / NUM_POINTS + M_PI / 2);
        pt->y = GAME_BUTTON_Y_RADIUS * sin(2 * M_PI * i / NUM_POINTS + M_PI / 2);
        list_add(points, pt);
    }
    polygon_translate(points, center);

    // body info
    char *info = malloc(7*sizeof(char));
    info[0] = '\0';
    strcat(info, "button");

    body_t *button = body_init_with_info(points, BUTTON_MASS, BUTTON_COLOR, info, free);
    return button;
}

// bool platform_overlap(scene_t *scene, body_t *body1, body_t *body2) { // can't remove without having their ints???
//     vector_t centroid1 = body_get_centroid(body1);
//     vector_t centorid2 = body_get_centroid(body2);
//     return (fabs(centroid1.x - centroid2.x) <= PLATFORM_WIDTH2 || fabs(centroid1.y - centroid2.y) <= PLATFORM_HEIGHT2);
// }

void more_platforms(scene_t *scene, vector_t center, int powerup_timer) {
    int num_platforms = 0;
    vector_t powerup_center;
    for (int i = 0; i < scene_bodies(scene); i++) {
        body_t *platform = scene_get_body(scene, i);
        char *info = body_get_info(platform);
        if (strstr(info, "platform") == NULL) {
            continue;
        }
        num_platforms++;
        char *info2 = body_get_second_info(platform);
        if (strcmp("essential", info2) == 0) {
            // essential platforms are generated one jump height apart
            double new_height = body_get_centroid(platform).y + GAME_MAX_JUMP;
            // only want to generate platforms that will be within one window height above the current window
            if ((new_height > center.y + GAME_HEIGHT/2) && (new_height < center.y + GAME_HEIGHT/2 + GAME_HEIGHT)) {
                strcat(info2, " done");
                vector_t platform_center = {.x = (double)rand()/RAND_MAX * (GAME_WIDTH - PLATFORM_WIDTH2) + PLATFORM_WIDTH2/2, .y = new_height};
                powerup_center = (vector_t) {.x = platform_center.x, .y = platform_center.y + 50};
                int random = rand()%4;
                char *new_info1;
                body_t *new_platform;
                if (random == 0) {
                    new_info1 = malloc(17*sizeof(char));
                    strcpy(new_info1, "sliding platform");
                    new_platform = sliding_platform(platform_center, new_info1);
                
                }
                else {
                    new_info1 = malloc(16*sizeof(char));
                    strcpy(new_info1, "normal platform");
                    new_platform = normal_platform(platform_center, new_info1);
                }
                char *new_info2 = malloc(15*sizeof(char));
                strcpy(new_info2, "essential");
                body_set_second_info(new_platform, new_info2);
                scene_add_body(scene, new_platform);
                create_platform_collision(scene, 0, scene_get_body(scene, 0), new_platform);
            }
        }
    }
    if (powerup_timer >= 950) {
        make_powerup(scene, powerup_center);
    }
    int i = num_platforms;
    int difficulty = 0;
    // int body_num = num_platforms; if i decide to make platforms not overlap
    if (center.y == -1 * GAME_HEIGHT/2) {
        i = (int)MAX_PLATFORMS/2 + 3;
    }
    else {
        difficulty += abs((int)center.y)/(int)GAME_HEIGHT/2;
        if (num_platforms == (int)(GAME_HEIGHT*2/GAME_MAX_JUMP)) {
            i = MAX_PLATFORMS - difficulty - 1;
        }
    }
    while (i < MAX_PLATFORMS - difficulty) {
        char *info2 = malloc(13*sizeof(char));
        strcpy(info2, "nonessential");
        int random = rand() % 8;
        vector_t platform_center = {.x = (double)rand()/RAND_MAX * (GAME_WIDTH - PLATFORM_WIDTH2) + PLATFORM_WIDTH2/2, .y = (double)rand()/RAND_MAX * GAME_HEIGHT + center.y + GAME_HEIGHT/2};
        char *info;
        body_t *new_platform;
        if (random == 0) {
            info = malloc(22*sizeof(char));
            strcpy(info, "disappearing platform");
            new_platform = disappearing_platform(platform_center, info);
        }
        else if (random < 2) {
            info = malloc(17*sizeof(char));
            strcpy(info, "sliding platform");
            new_platform = sliding_platform(platform_center, info);
        }
        else if (random < 5) {
            info = malloc(15*sizeof(char));
            strcpy(info, "trick platform");
            new_platform = trick_platform(platform_center, info);
        }
        else {
            info = malloc(16*sizeof(char));
            strcpy(info, "normal platform");
            new_platform = normal_platform(platform_center, info);
        }
        body_set_second_info(new_platform, info2);
        scene_add_body(scene, new_platform);
        create_platform_collision(scene, 0, scene_get_body(scene, 0), new_platform);
        i++;
    }
}

void more_enemies(scene_t *scene, vector_t center) {
    // if (within(5, ((int)round(center.y))%(int)HEIGHT2, 0) && ((int)round(center.y/(int)HEIGHT2))%2 == 0 && center.y != HEIGHT2/2) {
    if (rand()%4000 == 0) {
        vector_t centroid = {.x = (double)rand()/RAND_MAX * (GAME_WIDTH - GAME_DOODLE_HEIGHT) + GAME_DOODLE_HEIGHT/2, .y = center.y + (double)rand()/RAND_MAX * GAME_HEIGHT + GAME_HEIGHT/2};
        body_t *enemy = make_enemy(centroid);
        scene_add_body(scene, enemy);
        // add enemy collision here
    }
}

scene_t *make_game_scene() {
    char *game_info = malloc(5*sizeof(char));
    strcpy(game_info, "game");
    scene_t *scene = scene_init_with_info(game_info, free);
    // doodle
    char *doodle_info = malloc(7*sizeof(char));
    strcpy(doodle_info, "doodle");
    vector_t start = {.x = GAME_WIDTH/2, .y = GAME_START_VELOCITY.y/2 + GAME_DOODLE_HEIGHT/2};

    body_t *doodle = make_doodle(start, GAME_DOODLE_COLOR, doodle_info);

    sprite_t *right_jump = body_get_sprite(doodle);
    sprite_t *left_jump = make_jump_left();
    scene_add_sprite(scene, right_jump);
    scene_add_sprite(scene, left_jump);

    sprite_t *right_crouch = make_crouch_right();
    sprite_t *left_crouch = make_crouch_left();
    scene_add_sprite(scene, right_crouch);
    scene_add_sprite(scene, left_crouch);

    body_set_velocity(doodle, GAME_START_VELOCITY);
    scene_add_body(scene, doodle);
    // create_downward_gravity(scene, GAME_G, doodle);

    body_t *background1 = make_background_body((vector_t){.x = 0, .y = GAME_HEIGHT});
    body_t *background2 = make_background_body((vector_t){.x = 0, .y = 2*GAME_HEIGHT});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);

    vector_t platform_center = {.x = (double)rand()/RAND_MAX * (GAME_WIDTH - PLATFORM_WIDTH2) + PLATFORM_WIDTH2/2, .y = GAME_MAX_JUMP - GAME_DOODLE_HEIGHT/2 - PLATFORM_HEIGHT2/2};
    char *info = malloc(16*sizeof(char));
    strcpy(info, "normal platform");
    char *info2 = malloc(15*sizeof(char));
    strcpy(info2, "essential");
    body_t *platform = normal_platform(platform_center, info);
    body_set_second_info(platform, info2);
    scene_add_body(scene, platform);
    create_platform_collision(scene, 0, doodle, platform);
    vector_t safety_platform_center = {.x = GAME_WIDTH/2, .y = GAME_MAX_JUMP/2};
    char *other_info = malloc(16*sizeof(char));
    strcpy(other_info, "normal");
    char *other_info2 = malloc(15*sizeof(char));
    strcpy(other_info2, "essential");
    body_t *safety_platform = normal_platform(safety_platform_center, other_info);
    body_set_second_info(safety_platform, other_info2);
    scene_add_body(scene, safety_platform);
    create_platform_collision(scene, 0, doodle, safety_platform);

    vector_t center = {.x = GAME_WIDTH/2, .y = -1 * GAME_HEIGHT/2};
    more_platforms(scene, center, 0);

    return scene;
}

bool in_screen(vector_t center, body_t *body) {
    list_t *points = body_get_shape(body);
    // printf("%f\n", center.y);
    for (int i = 0; i < list_size(points); i++) {
        if (((vector_t *)list_get(points, i))->y > center.y - GAME_HEIGHT/2) {
            return true;
        }
    }
    return false;
}

body_t *wrap(body_t *doodle) {
    if (body_get_centroid(doodle).x >= GAME_WIDTH + GAME_DOODLE_WIDTH/2) {
        vector_t shift = {.x = 0, .y = body_get_centroid(doodle).y};
        body_set_centroid(doodle, shift);
    }
    else if (body_get_centroid(doodle).x <= 0 - GAME_DOODLE_WIDTH/2) {
        vector_t shift = {.x = GAME_WIDTH, .y = body_get_centroid(doodle).y};
        body_set_centroid(doodle, shift);
    }
    return doodle;
}

body_t *make_pellet (vector_t center) {
    list_t *points = list_init(20, free); // 13/16 + 1
    for (int i = 0; i < 20; i++) {
        vector_t *pt = malloc(sizeof(vector_t));
        pt->x = 7 * cos(2 * M_PI * i / 20 + M_PI / 2);
        pt->y = 7 * sin(2 * M_PI * i / 20 + M_PI / 2);
        list_add(points, pt);
    }
    char *info = malloc(7*sizeof(char));
    strcpy(info, "pellet");
    polygon_translate(points, center);
    body_t *pellet = body_init_with_info(points, 5, GAME_DOODLE_COLOR, info, free);
    return pellet;
}

void create_star(scene_t *scene) {
    // pick a random normal platform
    int random = 0;
    char *info = body_get_info(scene_get_body(scene, random));
    // SWITCH THIS TO NORMAL PLATFORMS LATER SOMEHOW
    bool conflict = true;
    while (strcmp("essential platform", info) != 0 && conflict) {
        random = rand() % scene_bodies(scene);
        info = body_get_info(scene_get_body(scene, random));

        // check if platform already has a star on it
        for (size_t j = 0; j < scene_bodies(scene); j++) {
            body_t *body1 = scene_get_body(scene, j);
            body_t *body2 = scene_get_body(scene, random);
            if (strcmp("star", body_get_info(scene_get_body(scene, j))) == 0 && random != j && body_get_centroid(body1).x == body_get_centroid(body2).x) {
                break;
            }
        }
        conflict = false;
    }
    body_t *platform = scene_get_body(scene, random);
    vector_t center = body_get_centroid(platform);
    center.y += 40; // magic number for offset

    star_t *starframe = make_star(center, 5, 17); // magic number for num points, radius
    rgb_color_t color = {.r = get_r(starframe), .g = get_g(starframe), .b = get_b(starframe)};
    char *star_info = malloc(5*sizeof(char));
    strcpy(star_info, "star");
    body_t *star = body_init_with_info(get_points(starframe), 0.5, color, star_info, free); // magic number for star mass

    scene_add_body(scene, star);
    return star;
}

double calculate_score(vector_t center) {
    // find doodle center height
    double height = center.y;

    // calculate score based on certain divisor or smth
    double score = height / SCORE_FACTOR;

    return score;
}

void game_mouse_click (scene_t *scene, int x, int y) {
    body_t *player = scene_get_body((scene_t *)scene, 0);
    vector_t mouth = find_mouth(player);
    vector_t mouth_window = get_window_position(mouth, get_window_center());
    body_t *pellet;
    pellet = make_pellet(mouth);
    body_set_velocity(pellet, (vector_t){.x = x-mouth_window.x, .y = -y+mouth_window.y});
    scene_add_body(scene, pellet);
    if (get_sound_preference()) {
        play_shoot();
    }
}

char *game_main (scene_t *scene, body_t *doodle, int *star_timer, int *powerup_timer, int *timer, vector_t *center) {
    // calculate and display score
    // generate a star once in a while
    
    rgb_color_t color = {.r = 0, .g = 0, .b = 0};
    bool enemy_present = false;
    vector_t *scoring = malloc(sizeof(vector_t));
    scoring->x = 80; // magic numbers
    scoring->y = 20;
    double curr = 0.0;

    char *score = malloc(100*sizeof(char));
    char *buffer = malloc(100*sizeof(char));

    if (*star_timer == 100) {
        create_star(scene);
        *star_timer = 0;
    }
    (*star_timer)++;
    if (scene_textboxes(scene) > 1) {
        scene_remove_text(scene, scene_get_text(scene, scene_textboxes(scene) - 1));
    }
    strcpy(score, "Score: ");
    curr = calculate_score(*center);

    sprintf(buffer, "%.1f", curr);
    strcat(score, buffer);
    if (get_score_preference()) {
        text_t *scorebox = text_create(score, color, 30, scoring);
        scene_add_text(scene, scorebox);
    }

    double dt = time_since_last_tick();
    (*powerup_timer)++;
    if (!in_screen(*center, doodle)) {
        char *restart_info = malloc(8*sizeof(char));
        strcpy(restart_info, "restart");
        scene_set_next_info(scene, restart_info);
    }

    for(int i = 3; i < scene_bodies(scene); i++) {
        body_t *body = scene_get_body(scene, i);
        if (!enemy_present && strcmp(body_get_info(body), "enemy") == 0) {
            enemy_present = true;
        }
        if (!in_screen(*center, body)) {
            scene_remove_body(scene, i);
        }
        if (strcmp(body_get_info(body), "sliding platform") == 0) {
            sliding_bounce(body);
        }
    }

    // shifting the viewing window if the doodle goes higher than the center
    if (body_get_centroid(doodle).y > center->y) {
        // generates more platforms
        more_platforms(scene, *center, *powerup_timer);
        if (*powerup_timer >= 1000) {
            *powerup_timer = 0;
        }
        if (!enemy_present) {
            more_enemies(scene, *center);
        }
        center->y = body_get_centroid(doodle).y;
        sdl_set_center(*center);
        for (int i = 1; i < 3; i++) {
            body_t *background = scene_get_body(scene, i);
            vector_t centroid = body_get_centroid(background);
            if (centroid.y <= center->y - GAME_HEIGHT/2) {
                centroid.y += GAME_HEIGHT*2;
                body_set_centroid(background, centroid);
            }
        }
    }
    enemy_present = false;

    if (body_get_sprite(doodle) == scene_get_sprite(scene, 2) || body_get_sprite(doodle) == scene_get_sprite(scene, 3)) {
        (*timer)++;
    }
    if (within(1, body_get_velocity(doodle).y, 299.1) && body_get_centroid(doodle).y > 75) { // magic numbers yikes
        if (body_get_direction(doodle) == 0) {
            change_motion(doodle, scene_get_sprite(scene, 2));
        }
        else {
            change_motion(doodle, scene_get_sprite(scene, 3));
        }
    }
    else if (*timer == 25) {
        if (body_get_direction(doodle) == 0) {
            change_motion(doodle, scene_get_sprite(scene, 0));
        }
        else {
            change_motion(doodle, scene_get_sprite(scene, 1));
        }
        *timer = 0;
    }
    wrap(doodle);
    scene_tick(scene, dt);
    sdl_render_scene(scene);
    return score;
}