#include "game.h"
#include "star.h"
#include "shop.h"
#include "sdl_wrapper.h"
#include "game_sprites.h"
#include "test_util.h"
#include "polygon.h"
#include "forces.h"
#include "constants.h"

const double MAX_PLATFORMS = 14;
const double PLATFORM_WIDTH2 = 146;
const double PLATFORM_HEIGHT2 = 35;
const vector_t PANEL_DIMENSTIONS = {.x = 500, .y = 500};
const int NUM_PANELS = 4;

const double SCORE_FACTOR = 20;

const double BUTTON_OFFSET = 100;
const vector_t PREVIOUS_BUTTON = {.x = 260.0, .y = 630.0};
const vector_t NEXT_BUTTON = {.x = 450.0, .y = 630.0};
const vector_t SCORE_MARKER_CENTER = {.x = 90, .y = 20};

const double POWERUP_TIMER = 1200;

body_t *make_enemy(vector_t center) {
    int idx = (rand() % (3 - 1 + 1)) + 1;
    list_t *shape;
    sprite_t *sprite = NULL;
    if (idx == 1) {
        vector_t bottom_left = {.x = center.x-1027/16, .y = center.y-1321/16};
        shape = make_rectangle(bottom_left, 1027/8, 1321/8);
        sprite = create_sprite("PNGs/Enemy_1.png", 1027/8, 1321/8);
    }
    if (idx == 2) {
        vector_t bottom_left = {.x = center.x-1072/16, .y = center.y-1300/16};
        shape = make_rectangle(bottom_left, 1072/8, 1300/8);
        sprite = create_sprite("PNGs/Enemy_2.png", 1072/8, 1300/8);
    }
    if (idx == 3) {
        vector_t bottom_left = {.x = center.x-905/16, .y = center.y-1399/16};
        shape = make_rectangle(bottom_left, 905/8, 1399/8);
        sprite = create_sprite("PNGs/Enemy_3.png", 905/8, 1399/8);
    }
    char *info = malloc(sizeof(char)*6);
    strcpy(info, "enemy");
    body_t *doodle = body_init_with_info(shape, INFINITY, DOODLE_COLOR, info, free);
    body_set_sprite(doodle, sprite);
    body_set_centroid(doodle, center);

    return doodle;
}

void platform_overlap(body_t *body1, body_t *body2) {
    if (body_is_removed(body1) || body_is_removed(body2)) {
        return;
    }
    vector_t centroid1 = body_get_centroid(body1);
    vector_t centroid2 = body_get_centroid(body2);
    if (fabs(centroid1.y - centroid2.y) <= PLATFORM_HEIGHT2) {
        if (strstr(body_get_second_info(body1), "nonessential") != NULL) {
            body_remove(body1);
        }
        else {
            body_remove(body2);
        }
    }
}

void more_platforms(scene_t *scene, vector_t center) {
    int num_platforms = 0;
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
            double new_height = body_get_centroid(platform).y + MAX_JUMP;
            // only want to generate platforms that will be within one window height above the current window
            if ((new_height > center.y + SCREEN_DIMENSIONS.y/2) && (new_height < center.y + SCREEN_DIMENSIONS.y/2 + SCREEN_DIMENSIONS.y)) {
                strcat(info2, " done");
                vector_t platform_center = {.x = (double)rand()/RAND_MAX * (SCREEN_DIMENSIONS.x - PLATFORM_WIDTH2) + PLATFORM_WIDTH2/2, .y = new_height};
                int random = rand()%40;
                char *new_info1;
                body_t *new_platform;
                if (random <= center.y/SCREEN_DIMENSIONS.y && random <= 35) {
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

    int i = num_platforms;
    int difficulty = 0;
    if (center.y == -1 * SCREEN_DIMENSIONS.y/2) {
        i = (int)MAX_PLATFORMS/2 + 3;
    }
    else {
        difficulty += abs((int)center.y)/(int)SCREEN_DIMENSIONS.y/2;
        if (num_platforms == (int)(SCREEN_DIMENSIONS.y*2/MAX_JUMP)) {
            i = MAX_PLATFORMS - difficulty - 1;
        }
    }
    while (i < MAX_PLATFORMS - difficulty) {
        char *info2 = malloc(13*sizeof(char));
        strcpy(info2, "nonessential");
        int random = rand() % 8;
        vector_t platform_center = {.x = (double)rand()/RAND_MAX * (SCREEN_DIMENSIONS.x - PLATFORM_WIDTH2) + PLATFORM_WIDTH2/2, .y = (double)rand()/RAND_MAX * SCREEN_DIMENSIONS.y + center.y + SCREEN_DIMENSIONS.y/2};
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
    for (int i = 3; i < scene_bodies(scene); i++) {
        body_t *body1 = scene_get_body(scene, i);
        if (strstr(body_get_info(body1), "platform") == NULL) {
            continue;
        }
        for (int j = i + 1; j < scene_bodies(scene); j++) {
            body_t *body2 = scene_get_body(scene, j);
            if (strstr(body_get_info(body2), "platform") != NULL) {
                platform_overlap(body1, body2);
            }
        }
    }
}

void more_enemies(scene_t *scene, vector_t center) {
    int random = rand()%50000;
    if (random < 15 && random <= center.y/SCREEN_DIMENSIONS.y) {
        int immunity_idx = -1;
        for (int i = 0; i < scene_bodies(scene); i++) {
            body_t *body = scene_get_body(scene, i);
            if (strcmp(body_get_info(body), "immunity") == 0) {
            }
            if (strcmp(body_get_info(body), "immunity") == 0 && body_get_second_info(body) != NULL && strcmp(body_get_second_info(body), "equipped") == 0) {
                immunity_idx = i;
            }
        }
        vector_t centroid = {.x = (double)rand()/RAND_MAX * (SCREEN_DIMENSIONS.x - DOODLE_HEIGHT) + DOODLE_HEIGHT/2, .y = center.y + (double)rand()/RAND_MAX * SCREEN_DIMENSIONS.y + SCREEN_DIMENSIONS.y/2};
        body_t *enemy = make_enemy(centroid);
        scene_add_body(scene, enemy);
        create_destructive_collision(scene, scene_get_body(scene, 0), enemy);
        if (immunity_idx != -1) {
            body_t *immunity = scene_get_body(scene, immunity_idx);
            create_immunity_collision(scene, 0, immunity, enemy);
        }
    }
}

scene_t *make_game_scene() {
    char *game_info = malloc(5*sizeof(char));
    strcpy(game_info, "game");
    scene_t *scene = scene_init_with_info(game_info, free);
    // doodle
    char *doodle_info = malloc(7*sizeof(char));
    strcpy(doodle_info, "doodle");
    vector_t start = {.x = SCREEN_DIMENSIONS.x/2, .y = START_VELOCITY.y/2 + DOODLE_HEIGHT/2};

    sprite_t *right_jump = create_sprite("PNGs/Jump_Right.png", 117, 207);
    sprite_t *left_jump = create_sprite("PNGs/Jump_Left.png", 117, 207);
    scene_add_sprite(scene, right_jump);
    scene_add_sprite(scene, left_jump);

    sprite_t *right_wing = create_sprite("PNGs/Winged_Right.png", 1262/8, 1653/8);
    sprite_t *left_wing = create_sprite("PNGs/Winged_Left.png", 1262/8, 1653/8);;
    scene_add_sprite(scene, right_wing);
    scene_add_sprite(scene, left_wing);

    body_t *doodle = make_doodle(start, doodle_info, right_jump);
    body_set_velocity(doodle, START_VELOCITY);
    scene_add_body(scene, doodle);
    create_downward_gravity(scene, G, doodle);

    body_t *background1 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = SCREEN_DIMENSIONS.y});
    body_t *background2 = make_background_body("PNGs/Game_Background.png", (vector_t){.x = 0, .y = 2*SCREEN_DIMENSIONS.y});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);

    vector_t platform_center = {.x = (double)rand()/RAND_MAX * (SCREEN_DIMENSIONS.x - PLATFORM_WIDTH2) + PLATFORM_WIDTH2/2, .y = MAX_JUMP - DOODLE_HEIGHT/2 - PLATFORM_HEIGHT2/2};
    char *info = malloc(16*sizeof(char));
    strcpy(info, "normal platform");
    char *info2 = malloc(15*sizeof(char));
    strcpy(info2, "essential");
    body_t *platform = normal_platform(platform_center, info);
    body_set_second_info(platform, info2);
    scene_add_body(scene, platform);
    create_platform_collision(scene, 0, doodle, platform);
    vector_t safety_platform_center = {.x = SCREEN_DIMENSIONS.x/2, .y = MAX_JUMP/2};
    char *other_info = malloc(16*sizeof(char));
    strcpy(other_info, "normal platform");
    char *other_info2 = malloc(13*sizeof(char));
    strcpy(other_info2, "nonessential");
    body_t *safety_platform = normal_platform(safety_platform_center, other_info);
    body_set_second_info(safety_platform, other_info2);
    scene_add_body(scene, safety_platform);
    create_platform_collision(scene, 0, doodle, safety_platform);

    vector_t center = {.x = SCREEN_DIMENSIONS.x/2, .y = -1 * SCREEN_DIMENSIONS.y/2};
    more_platforms(scene, center);
    return scene;
}

bool in_screen(vector_t center, body_t *body) {
    list_t *points = body_get_shape(body);
    for (int i = 0; i < list_size(points); i++) {
        if (((vector_t *)list_get(points, i))->y > center.y - SCREEN_DIMENSIONS.y/2) {
            list_free(points);
            return true;
        }
    }
    list_free(points);
    return false;
}

void wrap(body_t *doodle) {
    if (body_get_centroid(doodle).x >= SCREEN_DIMENSIONS.x + DOODLE_WIDTH/2) {
        vector_t shift = {.x = 0, .y = body_get_centroid(doodle).y};
        body_set_centroid(doodle, shift);
    }
    else if (body_get_centroid(doodle).x <= 0 - DOODLE_WIDTH/2) {
        vector_t shift = {.x = SCREEN_DIMENSIONS.x, .y = body_get_centroid(doodle).y};
        body_set_centroid(doodle, shift);
    }
}

body_t *make_pellet (vector_t center) {
    list_t *points = make_circle(7);
    char *info = malloc(7*sizeof(char));
    strcpy(info, "pellet");
    polygon_translate(points, center);
    body_t *pellet = body_init_with_info(points, 5, DOODLE_COLOR, info, free);
    return pellet;
}

bool first_time_play () {
    FILE* file = fopen("first_time.txt", "r");
    if (!file) {
        return false;
    }
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "YES\n") == 0) {
            fclose(file);
            return true;
        }
        else if (strcmp(line, "NO\n") == 0) {
            fclose(file);
            return false;
        }
    }
    fclose(file);
    return false;
}

void set_first_time() {
    FILE *file = fopen("first_time.txt", "w");
    if (!file) {
        return;
    }
    fprintf(file, "NO\n");
    fclose(file);
}

void create_star(scene_t *scene) {
    // pick a random normal platform
    int random = 0;
    char *info = body_get_info(scene_get_body(scene, random));
    bool conflict = false;
    int counter = 0;
    int magnet_idx = -1;
    do {
        if (counter == scene_bodies(scene)) {
            // too many stars for now, just return
            return;
        }

        int counter2 = 0;
        while (strcmp("normal platform", info) != 0 && counter2 < 30) {
            random = rand() % scene_bodies(scene);
            info = body_get_info(scene_get_body(scene, random));
            counter2++;
        }
        if (counter2 >= 30) {
            return;
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
            if (strcmp(body_get_info(body1), "magnet") == 0 && body_get_second_info(body1) != NULL && strcmp(body_get_second_info(body1), "equipped") == 0) {
                magnet_idx = j;
            }
        }
        counter++;
    }
    while (conflict);

    body_t *platform = scene_get_body(scene, random);
    vector_t center = body_get_centroid(platform);
    center.y += 40;
    if (center.y > body_get_centroid(scene_get_body(scene, 0)).y) {
        star_t *starframe = make_star(center, NUM_STAR_POINTS, STAR_RADIUS);
        char *star_info = malloc(5*sizeof(char));
        strcpy(star_info, "star");
        body_t *star = body_init_with_info(get_points(starframe), 0.001, STAR_COLOR, star_info, free);

        create_star_collision(scene, 0, scene_get_body(scene, 0), star);
        if (magnet_idx != -1) {
            create_magnetic_force(scene, 5000000, scene_get_body(scene, magnet_idx), star);
        }
        scene_add_body(scene, star);
    }
}


double calculate_score(vector_t center) {
    // find doodle center height
    double height = center.y;

    // calculate score based on certain divisor or smth
    double score = height / SCORE_FACTOR;

    return score;
}

void game_mouse_click (scene_t *scene, int x, int y) {
    body_t *doodle = scene_get_body((scene_t *)scene, 0);
    vector_t mouth = find_mouth(doodle);
    vector_t mouth_window = get_window_position(mouth, get_window_center());
    body_t *pellet;
    pellet = make_pellet(mouth);
    body_set_velocity(pellet, (vector_t){.x = 2*(x-mouth_window.x), .y = 2*(-y+mouth_window.y)});
    for (int i = 0; i < scene_bodies(scene); i++) {
        if (strcmp(body_get_info(scene_get_body(scene, i)), "enemy") == 0) {
            create_destructive_collision(scene, pellet, scene_get_body(scene, i));
        }
    }
    scene_add_body(scene, pellet);
    if (get_sound_preference()) {
        play_shoot();
    }
}

int get_current_panel(scene_t *scene) {
    body_t *panel = scene_get_body(scene, 0);
    sprite_t *sprite = body_get_sprite(panel);
    for (int i = 0; i < NUM_PANELS; i++) {
        if (scene_get_sprite(scene, i) == sprite) {
            return i;
        }
    }
    return -1;
}

void instructions_mouse_click (scene_t *scene, int x, int y, double button_x_radius, double button_y_radius) {
    body_t *panel = scene_get_body(scene, 0);
    vector_t center = {.x = SCREEN_DIMENSIONS.x/2, .y = SCREEN_DIMENSIONS.y/2};
    if (x < (PREVIOUS_BUTTON.x + button_x_radius) && x > (PREVIOUS_BUTTON.x - button_x_radius)) {
        if (y < (PREVIOUS_BUTTON.y + button_y_radius) && y > (PREVIOUS_BUTTON.y - button_y_radius)) {
            int i = get_current_panel(scene);
            if (i > 0) {
                body_set_sprite(panel, scene_get_sprite(scene, i-1));
                body_set_centroid(panel, center);
            }
        }
    }
    if (x < (NEXT_BUTTON.x + button_x_radius) && x > (NEXT_BUTTON.x - button_x_radius)) {
        if (y < (NEXT_BUTTON.y + button_y_radius) && y > (NEXT_BUTTON.y - button_y_radius)) {
            int i = get_current_panel(scene);
            if (i < 3) {
                body_set_sprite(panel, scene_get_sprite(scene, i+1));
                body_set_centroid(panel, center);
            }
            else if (i == 3) {
                char *game_info = malloc(5*sizeof(char));
                strcpy(game_info, "game");
                scene_set_next_info(scene, game_info);
                set_first_time();
            }
        }
    }
}

void star_score(scene_t *scene) {
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        if (body_get_second_info(scene_get_body(scene, i)) != NULL && strcmp(body_get_info(scene_get_body(scene, i)), "star") == 0 && strcmp(body_get_second_info(scene_get_body(scene, i)), "collected") == 0) {
            scene_increase_stars(scene);
            scene_remove_body(scene, i);
        }
    }
}

void star_updating(int star_score) {
    FILE *star_file = fopen("stars.txt", "r+");
    if (star_file == NULL) {
        printf("NULL file.\n");
    }
    char *star_reading = malloc(10*sizeof(char));
    char **throwaway = malloc(sizeof(char *));
    *throwaway = malloc(10*sizeof(char));
    if (fgets(star_reading, sizeof(star_reading), star_file) == NULL) {
        printf("Error.\n");
    }
    else {
        star_reading[6] = '\0';
        star_score += (int)strtod(star_reading, throwaway);
    }
    sprintf(star_reading, "%i", star_score);
    fseek(star_file, 0, SEEK_SET);
    fputs(star_reading, star_file);
    free(star_reading);
    fclose(star_file);
    free(throwaway);
}

void high_score_updating(char *score) {
    // only save score if it's a high score
    char **throwaway = malloc(sizeof(char *));
    *throwaway = malloc(10*sizeof(char));

    FILE *score_file = fopen("highscore.txt", "r+");
    if (score_file == NULL) {
        printf("NULL file.\n");
    }
    char *score_reading = malloc(100*sizeof(char));
    if (fgets(score_reading, sizeof(score_reading), score_file) == NULL) {
        printf("Error.\n");
        score+=7;
        fseek(score_file, 0, SEEK_SET);
        fputs(score, score_file);
    }
    else {
        score_reading[6] = '\0';
        double highscore = strtod(score_reading, throwaway);
        score+=7;
        double curr = strtod(score, throwaway);
        if (curr > highscore) {
            fseek(score_file, 0, SEEK_SET);
            fputs(score, score_file);
        }
    }
    fclose(score_file);
    free(score_reading);
    free(throwaway);
}

scene_t *make_instructions_scene() {
    char *game_info = malloc(13*sizeof(char));
    strcpy(game_info, "instructions");
    scene_t *scene = scene_init_with_info(game_info, free);


    sprite_t *first = create_sprite("PNGs/Game_Instructions_1.png", PANEL_DIMENSTIONS.x, PANEL_DIMENSTIONS.y);
    scene_add_sprite(scene, first);
    sprite_t *second = create_sprite("PNGs/Game_Instructions_2.png", PANEL_DIMENSTIONS.x, PANEL_DIMENSTIONS.y);
    scene_add_sprite(scene, second);
    sprite_t *third = create_sprite("PNGs/Game_Instructions_3.png", PANEL_DIMENSTIONS.x, PANEL_DIMENSTIONS.y);
    scene_add_sprite(scene, third);
    sprite_t *ready = create_sprite("PNGs/Game_Instructions_4.png", PANEL_DIMENSTIONS.x, PANEL_DIMENSTIONS.y);
    scene_add_sprite(scene, ready);
    vector_t center = {.x = SCREEN_DIMENSIONS.x/2, .y = SCREEN_DIMENSIONS.y/2};
    list_t *shape = make_rectangle(center, PANEL_DIMENSTIONS.x, PANEL_DIMENSTIONS.y);
    char *info = malloc(sizeof(char)*6);
    strcpy(info, "panel");
    body_t *panel = body_init_with_info(shape, INFINITY, DOODLE_COLOR, info, free);
    sprite_t *sprite = scene_get_sprite(scene, 0);
    body_set_sprite(panel, sprite);
    body_set_centroid(panel, center);
    scene_add_body(scene, panel);
    body_t *background1 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = SCREEN_DIMENSIONS.y});
    body_t *background2 = make_background_body("PNGs/Game_Background.png", (vector_t){.x = 0, .y = 2*SCREEN_DIMENSIONS.y});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);
    return scene;
}

void game_main (scene_t *scene, body_t *doodle, int *star_timer, int *powerup_timer, vector_t *center, char *score) {
    bool enemy_present = false;
    vector_t *scoring = malloc(sizeof(vector_t));
    scoring->x = SCORE_MARKER_CENTER.x;
    scoring->y = SCORE_MARKER_CENTER.y;
    double curr = 0.0;
    char *buffer = malloc(100*sizeof(char));
    double dt = time_since_last_tick();
    // generate a star once in a while
    if (*star_timer == 200) {
        create_star(scene);
        *star_timer = 0;
    }
    (*star_timer)++;
    star_score(scene);

    // calculate and display score
    if (scene_textboxes(scene) >= 1) {
        for (size_t i = 0; i < scene_textboxes(scene); i++) {
            scene_remove_text(scene, scene_get_text(scene, i));
        }
    }
    strcpy(score, "Score: ");
    curr = calculate_score(*center);

    sprintf(buffer, "%.1f", curr);
    strcat(score, buffer);
    if (get_score_preference()) {
        text_t *scorebox = text_create(score, TEXT_COLOR, 30, scoring);
        scene_add_text(scene, scorebox);
    }

    if (!in_screen(*center, doodle)) {
        star_updating(scene_stars(scene));
        high_score_updating(score);
        *powerup_timer = 0;
        *star_timer = 0;
        char *restart_info = malloc(8*sizeof(char));
        strcpy(restart_info, "restart");
        scene_set_next_info(scene, restart_info);
    }
    else {
        for(int i = 3; i < scene_bodies(scene); i++) {
            body_t *body = scene_get_body(scene, i);
            if (!enemy_present && (strcmp(body_get_info(body), "enemy") == 0 || strcmp(body_get_info(body), "boost") == 0)) {
                enemy_present = true;
            }
            if (!in_screen(*center, body)) {
                scene_remove_body(scene, i);
            }
            if (strcmp(body_get_info(body), "pellet") == 0 && body_get_centroid(body).y > center->y + SCREEN_DIMENSIONS.y/2) {
                scene_remove_body(scene, i);
            }
            if (strcmp(body_get_info(body), "sliding platform") == 0) {
                sliding_bounce(body);
            }
        }

        magnet_powerup(scene, powerup_timer);
        immunity_powerup(scene, powerup_timer);
        if (*powerup_timer == POWERUP_TIMER) {
            bool powerup_pref = get_powerup_preference();
            if (powerup_pref) {
                make_powerup(scene, enemy_present);
            }
            *powerup_timer = 0;
        }
        (*powerup_timer)++;

        for (int i = 0; i < scene_bodies(scene); i++) {
            body_t *body = scene_get_body(scene, i);
            body_t *doodle = scene_get_body(scene, 0);
            if (strcmp(body_get_info(body), "boost") == 0 && body_get_second_info(body) != NULL && strcmp(body_get_second_info(body), "equipped") == 0) {
                scene_remove_body(scene, i);
                char *new_info = malloc(sizeof(char) * 7);
                strcpy(new_info, "winged");
                body_set_second_info(doodle, new_info);
                if (body_get_direction(doodle) == 0) {
                    body_set_sprite(doodle, scene_get_sprite(scene, 2));
                }
                else {
                    body_set_sprite(doodle, scene_get_sprite(scene, 3));
                }
            }
        }

        if (body_get_second_info(doodle) != NULL && strcmp(body_get_second_info(doodle), "winged") == 0 && within(1, body_get_velocity(doodle).y, 0)) {
            char *new_info = malloc(sizeof(char) * 5);
            strcpy(new_info, "jump");
            body_set_second_info(doodle, new_info);
            if (body_get_direction(doodle) == 0) {
                    body_set_sprite(doodle, scene_get_sprite(scene, 0));
                }
                else {
                    body_set_sprite(doodle, scene_get_sprite(scene, 1));
                }
        }
        // shifting the viewing window if the doodle goes higher than the center
        if (body_get_centroid(doodle).y > center->y) {
            // generates more platforms
            more_platforms(scene, *center);
            center->y = body_get_centroid(doodle).y;
            sdl_set_center(*center);
            for (int i = 1; i < 3; i++) {
                body_t *background = scene_get_body(scene, i);
                vector_t centroid = body_get_centroid(background);
                if (centroid.y <= center->y - SCREEN_DIMENSIONS.y/2) {
                    centroid.y += SCREEN_DIMENSIONS.y*2;
                    body_set_centroid(background, centroid);
                }
            }
            if (!enemy_present) {
                more_enemies(scene, *center);
            }
        }
        enemy_present = false;

        if (*powerup_timer == POWERUP_TIMER) {
            for (int i = 0; i < scene_bodies(scene); i++) {
                body_t *body = scene_get_body(scene, i);
                if (strcmp(body_get_info(scene_get_body(scene, i)), "star") == 0) {
                    body_set_velocity(body, VEC_ZERO);
                }
                if (strcmp(body_get_info(body), "magnet") == 0 && body_get_second_info(body) != NULL && strcmp(body_get_second_info(body), "equipped") == 0) {
                    body_set_sprite(body, NULL);
                    scene_remove_body(scene, i);
                }
                if (strcmp(body_get_info(body), "immunity") == 0 && body_get_second_info(body) != NULL && strcmp(body_get_second_info(body), "equipped") == 0) {
                    scene_remove_body(scene, i);
                }
            }
        }
        wrap(doodle);
        scene_tick(scene, dt);
    }
    free(buffer);
    // bool first_time = first_time_play();
    // if (first_time) {
        // char *game_info = malloc(13*sizeof(char));
        // strcpy(game_info, "instructions");
        // scene_set_next_info(scene, game_info);
    // }
    // else {
    //     bool enemy_present = false;
    //     vector_t *scoring = malloc(sizeof(vector_t));
    //     scoring->x = SCORE_MARKER_CENTER.x;
    //     scoring->y = SCORE_MARKER_CENTER.y;
    //     double curr = 0.0;
    //     char *buffer = malloc(100*sizeof(char));
    //     double dt = time_since_last_tick();
    //     // generate a star once in a while
    //     if (*star_timer == 200) {
    //         create_star(scene);
    //         *star_timer = 0;
    //     }
    //     (*star_timer)++;
    //     star_score(scene);

    //     // calculate and display score
    //     if (scene_textboxes(scene) >= 1) {
    //         for (size_t i = 0; i < scene_textboxes(scene); i++) {
    //             scene_remove_text(scene, scene_get_text(scene, i));
    //         }
    //     }
    //     strcpy(score, "Score: ");
    //     curr = calculate_score(*center);

    //     sprintf(buffer, "%.1f", curr);
    //     strcat(score, buffer);
    //     if (get_score_preference()) {
    //         text_t *scorebox = text_create(score, TEXT_COLOR, 30, scoring);
    //         scene_add_text(scene, scorebox);
    //     }

    //     if (!in_screen(*center, doodle)) {
    //         star_updating(scene_stars(scene));
    //         high_score_updating(score);
    //         *powerup_timer = 0;
    //         *star_timer = 0;
    //         char *restart_info = malloc(8*sizeof(char));
    //         strcpy(restart_info, "restart");
    //         scene_set_next_info(scene, restart_info);
    //     }
    //     else {
    //         for(int i = 3; i < scene_bodies(scene); i++) {
    //             body_t *body = scene_get_body(scene, i);
    //             if (!enemy_present && (strcmp(body_get_info(body), "enemy") == 0 || strcmp(body_get_info(body), "boost") == 0)) {
    //                 enemy_present = true;
    //             }
    //             if (!in_screen(*center, body)) {
    //                 scene_remove_body(scene, i);
    //             }
    //             if (strcmp(body_get_info(body), "pellet") == 0 && body_get_centroid(body).y > center->y + SCREEN_DIMENSIONS.y/2) {
    //                 scene_remove_body(scene, i);
    //             }
    //             if (strcmp(body_get_info(body), "sliding platform") == 0) {
    //                 sliding_bounce(body);
    //             }
    //         }

    //         magnet_powerup(scene, powerup_timer);
    //         immunity_powerup(scene, powerup_timer);
    //         if (*powerup_timer == POWERUP_TIMER) {
    //             bool powerup_pref = get_powerup_preference();
    //             if (powerup_pref) {
    //                 make_powerup(scene, enemy_present);
    //             }
    //             *powerup_timer = 0;
    //         }
    //         (*powerup_timer)++;

    //         for (int i = 0; i < scene_bodies(scene); i++) {
    //             body_t *body = scene_get_body(scene, i);
    //             body_t *doodle = scene_get_body(scene, 0);
    //             if (strcmp(body_get_info(body), "boost") == 0 && body_get_second_info(body) != NULL && strcmp(body_get_second_info(body), "equipped") == 0) {
    //                 scene_remove_body(scene, i);
    //                 char *new_info = malloc(sizeof(char) * 7);
    //                 strcpy(new_info, "winged");
    //                 body_set_second_info(doodle, new_info);
    //                 if (body_get_direction(doodle) == 0) {
    //                     body_set_sprite(doodle, scene_get_sprite(scene, 2));
    //                 }
    //                 else {
    //                     body_set_sprite(doodle, scene_get_sprite(scene, 3));
    //                 }
    //             }
    //         }

    //         if (body_get_second_info(doodle) != NULL && strcmp(body_get_second_info(doodle), "winged") == 0 && within(1, body_get_velocity(doodle).y, 0)) {
    //             char *new_info = malloc(sizeof(char) * 5);
    //             strcpy(new_info, "jump");
    //             body_set_second_info(doodle, new_info);
    //             if (body_get_direction(doodle) == 0) {
    //                     body_set_sprite(doodle, scene_get_sprite(scene, 0));
    //                 }
    //                 else {
    //                     body_set_sprite(doodle, scene_get_sprite(scene, 1));
    //                 }
    //         }
    //         // shifting the viewing window if the doodle goes higher than the center
    //         if (body_get_centroid(doodle).y > center->y) {
    //             // generates more platforms
    //             more_platforms(scene, *center);
    //             center->y = body_get_centroid(doodle).y;
    //             sdl_set_center(*center);
    //             for (int i = 1; i < 3; i++) {
    //                 body_t *background = scene_get_body(scene, i);
    //                 vector_t centroid = body_get_centroid(background);
    //                 if (centroid.y <= center->y - SCREEN_DIMENSIONS.y/2) {
    //                     centroid.y += SCREEN_DIMENSIONS.y*2;
    //                     body_set_centroid(background, centroid);
    //                 }
    //             }
    //             if (!enemy_present) {
    //                 more_enemies(scene, *center);
    //             }
    //         }
    //         enemy_present = false;

    //         if (*powerup_timer == POWERUP_TIMER) {
    //             for (int i = 0; i < scene_bodies(scene); i++) {
    //                 body_t *body = scene_get_body(scene, i);
    //                 if (strcmp(body_get_info(scene_get_body(scene, i)), "star") == 0) {
    //                     body_set_velocity(body, VEC_ZERO);
    //                 }
    //                 if (strcmp(body_get_info(body), "magnet") == 0 && body_get_second_info(body) != NULL && strcmp(body_get_second_info(body), "equipped") == 0) {
    //                     body_set_sprite(body, NULL);
    //                     scene_remove_body(scene, i);
    //                 }
    //                 if (strcmp(body_get_info(body), "immunity") == 0 && body_get_second_info(body) != NULL && strcmp(body_get_second_info(body), "equipped") == 0) {
    //                     scene_remove_body(scene, i);
    //                 }
    //             }
    //         }
    //         wrap(doodle);
    //         scene_tick(scene, dt);
    //     }
    //     free(buffer);
    // }
    sdl_render_scene(scene);
}