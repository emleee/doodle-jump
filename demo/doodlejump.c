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

const double WIDTH2 = 720.0;
const double HEIGHT2 = 960.0;
const double DOODLE_WIDTH = 96.0;
const double DOODLE_HEIGHT = 148.0;
const int NUM_POINTS = 50;
const double BUTTON_X_RADIUS = 125;
const double BUTTON_Y_RADIUS = 75;

const double MAX_PLATFORMS = 18;
const double PLATFORM_WIDTH2 = 60;
const double PLATFORM_HEIGHT2 = 20;

const vector_t START_VELOCITY = {.x = 0, .y = 300};
const double PLAYER_X_VELOCITY = 600;

const double SCORE_FACTOR = 20;

const rgb_color_t DOODLE_BODY_COLOR = {.r = 176.0/255, .g = 128.0/255, .b = 124.0/255};
const rgb_color_t BUTTON_COLOR = {.r = 255.0/255, .g = 255.0/255, .b = 255.0/255};
const double DOODLE_MASS = 5.0;
const double BUTTON_MASS = INFINITY;
const double MAX_JUMP = 290.0;

const double G = -150.0;

body_t *make_doodle(vector_t center, rgb_color_t color, char *info) {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {0, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {DOODLE_WIDTH, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {DOODLE_WIDTH, DOODLE_HEIGHT};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {0, DOODLE_HEIGHT};
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

// bool platform_overlap(scene_t *scene, body_t *body1, body_t *body2) { // can't remove without having their ints???
//     vector_t centroid1 = body_get_centroid(body1);
//     vector_t centorid2 = body_get_centroid(body2);
//     return (fabs(centroid1.x - centroid2.x) <= PLATFORM_WIDTH2 || fabs(centroid1.y - centroid2.y) <= PLATFORM_HEIGHT2);
// }

body_t *make_button(vector_t center) {
    // body shape
    list_t *points = list_init(NUM_POINTS, free);
    for (int i = 0; i < (NUM_POINTS); i++) {
        vector_t *pt = malloc(sizeof(vector_t));
        pt->x = BUTTON_X_RADIUS * cos(2 * M_PI * i / NUM_POINTS + M_PI / 2);
        pt->y = BUTTON_Y_RADIUS * sin(2 * M_PI * i / NUM_POINTS + M_PI / 2);
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

void more_platforms(scene_t *scene, vector_t center, bool first) {
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
            // only want to generate platforms that will be within one window height above the current window
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
    int i = num_platforms;
    int body_num = num_platforms;
    if (first) {
        i = MAX_PLATFORMS/2;
    }
    while (i < MAX_PLATFORMS) {
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
        i++;
    }
}

scene_t *make_game_scene() {
    char *game_info = malloc(5*sizeof(char));
    strcpy(game_info, "game");
    scene_t *scene = scene_init_with_info(game_info, free);
    // doodle
    char *doodle_info = malloc(7*sizeof(char));
    strcpy(doodle_info, "doodle");
    vector_t start = {.x = WIDTH2/2, .y = 0};

    body_t *doodle = make_doodle(start, DOODLE_BODY_COLOR, doodle_info);

    sprite_t *right_jump = body_get_sprite(doodle);
    sprite_t *left_jump = make_jump_left();
    scene_add_sprite(scene, right_jump);
    scene_add_sprite(scene, left_jump);

    sprite_t *right_crouch = make_crouch_right();
    sprite_t *left_crouch = make_crouch_left();
    scene_add_sprite(scene, right_crouch);
    scene_add_sprite(scene, left_crouch);

    body_set_velocity(doodle, START_VELOCITY);
    scene_add_body(scene, doodle);
    create_downward_gravity(scene, G, doodle);

    body_t *background1 = make_background_body((vector_t){.x = 0, .y = HEIGHT2});
    body_t *background2 = make_background_body((vector_t){.x = 0, .y = 2*HEIGHT2});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);

    vector_t platform_center = {.x = (double)rand()/RAND_MAX * (WIDTH2 - PLATFORM_WIDTH2), .y = MAX_JUMP - DOODLE_HEIGHT/2 - PLATFORM_HEIGHT2/2};
    char *info = malloc(24*sizeof(char));
    strcpy(info, "essential platform");
    body_t *platform = normal_platform(platform_center, info);
    scene_add_body(scene, platform);
    create_platform_collision(scene, 0, doodle, platform);
    vector_t center = {.x = WIDTH2/2, .y = -1 * HEIGHT2/2};
    more_platforms(scene, center, true);

    return scene;
}

scene_t *make_start_scene() {
    char *scene_info = malloc(6*sizeof(char));
    strcpy(scene_info, "start");
    scene_t *scene = scene_init_with_info(scene_info, free);

    rgb_color_t color = {.r = 0, .g = 0, .b = 0};
    vector_t *point2 = malloc(sizeof(vector_t));
    point2->x = 250; // remove magic numbers
    point2->y = 200;
    text_t *text2 = text_create("Start", color, 22, point2, 200, 30);
    scene_add_text(scene, text2);

    // body_t *start_button = make_button(*point2);
    // scene_add_body(scene, start_button);

    body_t *background1 = make_background_body((vector_t){.x = 0, .y = HEIGHT2});
    body_t *background2 = make_background_body((vector_t){.x = 0, .y = 2*HEIGHT2});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);
    return scene;
}

scene_t *make_restart_scene() {
    char *scene_info = malloc(8*sizeof(char));
    strcpy(scene_info, "restart");
    scene_t *scene = scene_init_with_info(scene_info, free);

    rgb_color_t color = {.r = 0, .g = 0, .b = 0};
    vector_t *point = malloc(sizeof(vector_t));
    point->x = 250; // remove magic numbers
    point->y = 200;
    text_t *text = text_create("Restart", color, 22, point, 200, 30);
    scene_add_text(scene, text);

    vector_t *point2 = malloc(sizeof(vector_t));
    point2->x = 250; // remove magic numbers
    point2->y = 500;
    text_t *text2 = text_create("Home", color, 22, point2, 200, 30);
    scene_add_text(scene, text2);

    // body_t *start_button = make_button(*point2);
    // scene_add_body(scene, start_button);

    body_t *background1 = make_background_body((vector_t){.x = 0, .y = HEIGHT2});
    body_t *background2 = make_background_body((vector_t){.x = 0, .y = 2*HEIGHT2});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);
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
    if (body_get_centroid(doodle).x >= WIDTH2 + DOODLE_WIDTH/2) {
        vector_t shift = {.x = 0, .y = body_get_centroid(doodle).y};
        body_set_centroid(doodle, shift);
    }
    else if (body_get_centroid(doodle).x <= 0 - DOODLE_WIDTH/2) {
        vector_t shift = {.x = WIDTH2, .y = body_get_centroid(doodle).y};
        body_set_centroid(doodle, shift);
    }
    return doodle;
}

void on_key(char key, key_event_type_t type, double held_time, void *scene) {
    body_t *player = scene_get_body((scene_t *)scene, 0);
    vector_t body_velocity = body_get_velocity(player);
    // double mass = body_get_mass(player);
    if (type == KEY_PRESSED) {
        switch (key) {
            case RIGHT_ARROW:
                if (body_get_sprite(player) == scene_get_sprite(scene, 1)) {
                    change_direction(player, scene_get_sprite(scene, 0));
                }
                body_set_rotation(player, 0);
                body_velocity.x = PLAYER_X_VELOCITY;
                body_set_velocity(player, body_velocity);
                // if (loadMedia()) {
                //     Mix_Chunk *jump = (Mix_Chunk *) get_jump();
                //     Mix_PlayChannel( -1, jump, 0 );
                // }
                break;
            case LEFT_ARROW:
                if (body_get_sprite(player) == scene_get_sprite(scene, 0)) {
                    change_direction(player, scene_get_sprite(scene, 1));
                }
                body_set_rotation(player, M_PI);
                body_velocity.x = -1 * PLAYER_X_VELOCITY;
                body_set_velocity(player, body_velocity);
                // if (loadMedia()) {
                //     Mix_Chunk *jump = (Mix_Chunk *) get_jump();
                //     Mix_PlayChannel( -1, jump, 0 );
                // }
                break;
        }
    }
    else {
        body_velocity.x = 0;
        body_set_velocity(player, body_velocity);
    }
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
    body_t *pellet = body_init_with_info(points, 5, DOODLE_BODY_COLOR, info, free);
    return pellet;
}

void mouse_click(int key, int x, int y, void *scene) {
    switch(key) {
        case SDL_BUTTON_LEFT:
            if (strcmp(scene_get_info(scene), "game") == 0) {
                body_t *player = scene_get_body((scene_t *)scene, 0);
                vector_t mouth = find_mouth(player);
                vector_t mouth_window = get_window_position(mouth, get_window_center());
                body_t *pellet;
                pellet = make_pellet(mouth);
                body_set_velocity(pellet, (vector_t){.x = x-mouth_window.x, .y = -y+mouth_window.y});
                scene_add_body(scene, pellet);
                if (loadMedia()) {
                    Mix_Chunk *shoot = (Mix_Chunk *) get_shoot();
                    Mix_PlayChannel( -1, shoot, 0 );
                }
            }
            else if (strcmp(scene_get_info(scene), "start") == 0) {
                if (x < (250 + BUTTON_X_RADIUS) && x > (250 - BUTTON_X_RADIUS)) {
                    if (y < (200 + BUTTON_Y_RADIUS) && y > (200 - BUTTON_Y_RADIUS)) {
                        char *game_info = malloc(5*sizeof(char));
                        strcpy(game_info, "game");
                        scene_set_next_info(scene, game_info);
                    }
                }
            }
            else if (strcmp(scene_get_info(scene), "restart") == 0) {
                if (x < (250 + BUTTON_X_RADIUS) && x > (250 - BUTTON_X_RADIUS)) {
                    if (y < (200 + BUTTON_Y_RADIUS) && y > (200 - BUTTON_Y_RADIUS)) {
                        char *game_info = malloc(5*sizeof(char));
                        strcpy(game_info, "game");
                        scene_set_next_info(scene, game_info);
                    }
                    else if (y < (500 + BUTTON_Y_RADIUS) && y > (500 - BUTTON_Y_RADIUS)) {
                        char *start_info = malloc(6*sizeof(char));
                        strcpy(start_info, "start");
                        scene_set_next_info(scene, start_info);
                    }
                }
            }
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

    int timer = 0;

    sdl_on_key(on_key);
    sdl_mouse(mouse_click);
    scene_t *scene = make_start_scene();
    
    
    vector_t center = {.x = WIDTH2/2, HEIGHT2/2};
    rgb_color_t color = {.r = 0, .g = 0, .b = 0};

    vector_t *scoring = malloc(sizeof(vector_t));
    scoring->x = 10;
    scoring->y = 10;

    char *score = malloc(100*sizeof(char));
    char *buffer = malloc(100*sizeof(char));

    body_t *doodle;            

    // char score[100];

    // score = "High Score: ";
    // strcat(score, snprintf(score, 50, "%d", calculate_score(scene)));
    // FILE *file = fopen("highscores.txt", "w");
    // fprintf(file, score);


    // score = calculate_score(scene);
    // text_t *score = text_create(score, color, 20, scoring, 20, 20);

    while (!sdl_is_done(scene)) {
        if (strcmp(scene_get_info(scene), scene_get_next_info(scene)) != 0) { 
            if (strcmp(scene_get_next_info(scene), "game") == 0) {
                scene_free(scene);
                scene = make_game_scene();
                doodle = scene_get_body(scene, 0);

                
                vector_t *point = malloc(sizeof(vector_t));
                point->x = 250; // remove magic numbers
                point->y = 10;
                text_t *text = text_create("Doodle Jump: Fairy Tail", color, 22, point, 200, 25);
                scene_add_text(scene, text);
            }
            else if (strcmp(scene_get_next_info(scene), "start") == 0) {
                scene_free(scene);
                scene = make_start_scene();
            }
            else if (strcmp(scene_get_next_info(scene), "restart") == 0) {
                scene_free(scene);
                scene = make_restart_scene();
            }
        }
        if (strcmp(scene_get_info(scene), "game") == 0) {
            // calculate and display score
            if (scene_textboxes(scene) > 1) {
                scene_remove_text(scene, scene_get_text(scene, scene_textboxes(scene) - 1));
            }
            strcpy(score, "High Score: ");
            double curr = calculate_score(center);

            sprintf(buffer, "%.1f", curr);
            strcat(score, buffer);
            text_t *scorebox = text_create(score, color, 40, scoring, 200, 40);
            scene_add_text(scene, scorebox);

            double dt = time_since_last_tick();

            if (!in_screen(center, doodle)) {
                // PLAYER LOSES, REPLACE BREAK WITH ACTUAL CODE
                // break;
                char *restart_info = malloc(8*sizeof(char));
                strcpy(restart_info, "restart");
                scene_set_next_info(scene, restart_info);
            }

            // shifting the viewing window if the doodle goes higher than the center
            if (body_get_centroid(doodle).y > center.y) {
                // generates more platforms
                more_platforms(scene, center, false);
                center.y = body_get_centroid(doodle).y;
                sdl_set_center(center);
                for (int i = 1; i < 3; i++) {
                    body_t *background = scene_get_body(scene, i);
                    vector_t centroid = body_get_centroid(background);
                    if (centroid.y <= center.y - HEIGHT2/2) {
                        centroid.y = center.y + HEIGHT2/2 + HEIGHT2;
                        body_set_centroid(background, centroid);
                    }
                }
            }

            if (body_get_sprite(doodle) == scene_get_sprite(scene, 2) || body_get_sprite(doodle) == scene_get_sprite(scene, 3)) {
                timer++;
            }
            if (within(1, body_get_velocity(doodle).y, 299.1) && body_get_centroid(doodle).y > 75) { // magic numbers yikes
                if (body_get_direction(doodle) == 0) {
                    change_motion(doodle, scene_get_sprite(scene, 2));
                }
                else {
                    change_motion(doodle, scene_get_sprite(scene, 3));
                }
            }
            else if (timer == 25) {
                if (body_get_direction(doodle) == 0) {
                    change_motion(doodle, scene_get_sprite(scene, 0));
                }
                else {
                    change_motion(doodle, scene_get_sprite(scene, 1));
                }
                timer = 0;
            }
            
            wrap(doodle);
            scene_tick(scene, dt);
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "start") == 0) {
            sdl_render_scene(scene);
        }
        else if (strcmp(scene_get_info(scene), "restart") == 0) {
            sdl_render_scene(scene);
        }
    }

    FILE *file = fopen("highscores.txt", "a+");
    fputs("\n", file);
    fprintf(file, score);

    fclose(file);
    free(score);
    free(buffer);
    scene_free(scene);
}
