#include "shop.h"
#include "color.h"
#include "preferences.h"
#include "game.h"
#include "restart.h"
#include "start.h"
#include "constants.h"

// locations
const vector_t BOOST_CENTER = {.x = 275, .y = 605};
const vector_t IMMUNITY_CENTER = {.x = 425, .y = 605};
const vector_t MAGNET_CENTER = {.x = 605, .y = 605};
const vector_t SHOP_HOME_CENTER = {.x = 425, .y = 765};
const vector_t STAR_COUNT_CENTER = {.x = 125, .y = 40};
const vector_t SIGN_LOCATION = {.x = 425, .y = 590};

// button dimenstions
const vector_t BUY_BUTTON_SIZE = {.x = 50, .y = 15};
const vector_t SHOP_HOME_BUTTON_SIZE = {.x = 85, .y = 20};
const vector_t SIGN_DIMENSIONS = {.x = 390, 150};

// star parameters
const double SHOP_STAR_RADIUS = 25;

// powerup prices
const int BOOST_PRICE = 100;
const int IMMUNITY_PRICE = 50;
const int MAGNET_PRICE = 80;

scene_t *make_shop_scene () {
    char *scene_info = malloc(5 * sizeof(char));
    strcpy(scene_info, "shop");
    scene_t *scene = scene_init_with_info(scene_info, free);

    body_t *background = make_background_body("PNGs/Shop_Background.png",(vector_t){.x = 0, .y = SCREEN_DIMENSIONS.y});
    scene_add_body(scene, background);

    display_star_count(scene);

    return scene;
}

void display_star_count(scene_t *scene) {
    // star count text
    char *stars = malloc(10*sizeof(char));
    sprintf(stars, "%i", get_star_count());
    vector_t *position = malloc(sizeof(vector_t));
    position->x = STAR_COUNT_CENTER.x;
    position->y = STAR_COUNT_CENTER.y;
    text_t *star_count = text_create(stars, TEXT_COLOR, 50, position);
    scene_add_text(scene, star_count);

    // star body
    vector_t star_pos = {.x = STAR_OFFSET, .y = SCREEN_DIMENSIONS.y - STAR_OFFSET};
    star_t *star_frame = make_star(star_pos, NUM_STAR_POINTS, SHOP_STAR_RADIUS);
    char *star_info = malloc(5 * sizeof(char));
    strcpy(star_info, "star");
    body_t *star = body_init_with_info(get_points(star_frame), 0.001, STAR_COLOR, star_info, free);
    scene_add_body(scene, star);
}

scene_t *make_shop_exit_scene() {
    char *scene_info = malloc(10 * sizeof(char));
    strcpy(scene_info, "shop exit");
    scene_t *scene = scene_init_with_info(scene_info, free);
    list_t *shape = make_rectangle(SIGN_LOCATION, SIGN_DIMENSIONS.x, SIGN_DIMENSIONS.y);
    char *info = malloc(sizeof(char)*5);
    strcpy(info, "exit");
    body_t *thanks = body_init_with_info(shape, INFINITY, TEXT_COLOR, info, free);
    sprite_t *sprite = create_sprite("PNGs/Thank_You_Sign.png", SIGN_DIMENSIONS.x, SIGN_DIMENSIONS.y);
    body_set_sprite(thanks, sprite);
    body_set_centroid(thanks, SIGN_LOCATION);
    scene_add_body(scene, thanks);
    body_t *background = make_background_body("PNGs/Shop_Background.png",(vector_t){.x = 0, .y = SCREEN_DIMENSIONS.y});
    scene_add_body(scene, background);
    return scene;
}

scene_t *make_failed_purchase_scene() {
    char *scene_info = malloc(10 * sizeof(char));
    strcpy(scene_info, "shop fail");
    scene_t *scene = scene_init_with_info(scene_info, free);
    list_t *shape = make_rectangle(SIGN_LOCATION, SIGN_DIMENSIONS.x, SIGN_DIMENSIONS.y);
    char *info = malloc(sizeof(char) * 5);
    strcpy(info, "exit");
    body_t *thanks = body_init_with_info(shape, INFINITY, TEXT_COLOR, info, free);
    sprite_t *sprite = create_sprite("PNGs/Sorry_Sign.png", SIGN_DIMENSIONS.x, SIGN_DIMENSIONS.y);
    body_set_sprite(thanks, sprite);
    body_set_centroid(thanks, SIGN_LOCATION);
    scene_add_body(scene, thanks);
    body_t *background = make_background_body("PNGs/Shop_Background.png",(vector_t){.x = 0, .y = SCREEN_DIMENSIONS.y});
    scene_add_body(scene, background);
    return scene;
}

void shop_mouse_click(scene_t *scene, int x, int y) {
    if (x < (BOOST_CENTER.x + BUY_BUTTON_SIZE.x) && x > (BOOST_CENTER.x - BUY_BUTTON_SIZE.x)) {
        if (y < (BOOST_CENTER.y + BUY_BUTTON_SIZE.y) && y > (BOOST_CENTER.y - BUY_BUTTON_SIZE.y)) {
            buy_boost(scene);
        }
    }
    if (x < (MAGNET_CENTER.x + BUY_BUTTON_SIZE.x) && x > (MAGNET_CENTER.x - BUY_BUTTON_SIZE.x)) {
        if (y < (MAGNET_CENTER.y + BUY_BUTTON_SIZE.y) && y > (MAGNET_CENTER.y - BUY_BUTTON_SIZE.y)) {
            buy_magnet(scene);
        }
    }
    if (x < (IMMUNITY_CENTER.x + BUY_BUTTON_SIZE.x) && x > (IMMUNITY_CENTER.x - BUY_BUTTON_SIZE.x)) {
        if (y < (IMMUNITY_CENTER.y + BUY_BUTTON_SIZE.y) && y > (IMMUNITY_CENTER.y - BUY_BUTTON_SIZE.y)) {
            buy_immunity(scene);
        }
    }
    if (x < (SHOP_HOME_CENTER.x + SHOP_HOME_BUTTON_SIZE.x) && x > (SHOP_HOME_CENTER.x - SHOP_HOME_BUTTON_SIZE.x)) {
        if (y < (SHOP_HOME_CENTER.y + SHOP_HOME_BUTTON_SIZE.y) && y > (SHOP_HOME_CENTER.y - SHOP_HOME_BUTTON_SIZE.y)) {
            char *start_info = malloc(6 * sizeof(char));
            strcpy(start_info, "start");
            scene_set_next_info(scene, start_info);
        }
    }
}

int get_star_count() {
    FILE *star_file = fopen("stars.txt", "r+");
    if (star_file == NULL) {
        printf("NULL file.\n");
        return 1;
    }
    char *star_reading = malloc(10 * sizeof(char));
    char **throwaway = malloc(sizeof(char *));
    *throwaway = malloc(10*sizeof(char));
    int num_stars;
    if (fgets(star_reading, sizeof(star_reading), star_file) == NULL) {
        printf("Error.\n");
        return 1;
    }
    else {
        star_reading[6] = '\0';
        num_stars = (int)strtod(star_reading, throwaway);
    }
    free(star_reading);
    free(throwaway);
    fclose(star_file);
    return num_stars;
}

void change_star_count(int new_count) {
    FILE *star_file = fopen("stars.txt", "w");
    if (star_file == NULL) {
        printf("NULL file.\n");
    }
    char *star_reading = malloc(10*sizeof(char));
    sprintf(star_reading, "%i", new_count);
    fseek(star_file, 0, SEEK_SET);
    fputs(star_reading, star_file);
    free(star_reading);
    fclose(star_file);
}

void write_powerup(char *powerup) {
    FILE *powerup_file = fopen("inventory.txt", "w");
    if (powerup_file == NULL) {
        printf("NULL file.\n");
    }
    fputs(powerup, powerup_file);
    fclose(powerup_file);
}

void buy_immunity(scene_t *scene) {
    int num_stars = get_star_count();
    if (num_stars < IMMUNITY_PRICE) {
        char *fail_info = malloc(10 * sizeof(char));
        strcpy(fail_info, "shop fail");
        scene_set_next_info(scene, fail_info);
    }
    else {
        if (get_sound_preference()) {
            play_cha_ching();
        }
        num_stars -= IMMUNITY_PRICE;
        change_star_count(num_stars);
        char *immunity = malloc(9 * sizeof(char));
        immunity = "immunity";
        write_powerup(immunity);
        char *exit_info = malloc(10 * sizeof(char));
        strcpy(exit_info, "shop exit");
        scene_set_next_info(scene, exit_info);
    }
}

void buy_magnet(scene_t *scene){
    int num_stars = get_star_count();
    if (num_stars < MAGNET_PRICE) {
        char *fail_info = malloc(10 * sizeof(char));
        strcpy(fail_info, "shop fail");
        scene_set_next_info(scene, fail_info);
    }
    else {
        if (get_sound_preference()) {
            play_cha_ching();
        }
        num_stars -= MAGNET_PRICE;
        change_star_count(num_stars);
        char *magnet = malloc(7 * sizeof(char));
        magnet = "magnet";
        write_powerup(magnet);
        char *exit_info = malloc(10 * sizeof(char));
        strcpy(exit_info, "shop exit");
        scene_set_next_info(scene, exit_info);
    }
}

void buy_boost(scene_t *scene){
    int num_stars = get_star_count();
    if (num_stars < BOOST_PRICE) {
        char *fail_info = malloc(10 * sizeof(char));
        strcpy(fail_info, "shop fail");
        scene_set_next_info(scene, fail_info);
    }
    else { 
        if (get_sound_preference()) {
            play_cha_ching();
        }
        num_stars -= BOOST_PRICE;
        change_star_count(num_stars);
        char *boost = malloc(6*sizeof(char));
        boost = "boost";
        write_powerup(boost);
        char *exit_info = malloc(10 * sizeof(char));
        strcpy(exit_info, "shop exit");
        scene_set_next_info(scene, exit_info);
    }
}

bool check_inventory () {
    FILE* file = fopen("inventory.txt", "r");
    if (!file) {
        return false;
    }
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "boost") == 0 || strcmp(line, "magnet") == 0 || strcmp(line, "immunity") == 0) {
            return true;
        }
    }
    fclose(file);
    return false;
}

void use_inventory (scene_t *scene) {
    body_t *doodle = scene_get_body(scene, 0);
    vector_t center = body_get_centroid(doodle);
    FILE* file = fopen("inventory.txt", "r");
    if (!file) {
        return;
    }
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "boost") == 0) {
            make_boost(scene, center);
            fclose(file);
            file = fopen("inventory.txt", "w");
        }
        else if (strcmp(line, "magnet") == 0) {
            make_magnet(scene, center, false);
            fclose(file);
            file = fopen("inventory.txt", "w");
        }
        else if (strcmp(line, "immunity") == 0) {
            make_immunity(scene, center, false);
            fclose(file);
            file = fopen("inventory.txt", "w");
        }
    }
    fclose(file);
}