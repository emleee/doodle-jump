#include "shop.h"

const vector_t BOOST_CENTER = {.x = 360, .y = 245};
const vector_t IMMUNITY_CENTER = {.x = 360, .y = 425};
const vector_t MAGNET_CENTER = {.x = 360, .y = 588};
const vector_t HOME_CENTER = {.x = 360, .y = 650};

const double SHOP_WIDTH = 720.0;
const double SHOP_HEIGHT = 960.0;

const int BOOST_PRICE = 100;
const int IMMUNITY_PRICE = 50;
const int MAGNET_PRICE = 80;

scene_t *make_shop_scene () {
    char *scene_info = malloc(5*sizeof(char));
    strcpy(scene_info, "shop");
    scene_t *scene = scene_init_with_info(scene_info, free);

    body_t *boost = make_boost(scene, BOOST_CENTER);
    body_t *immunity = make_immunity(scene, IMMUNITY_CENTER, false);
    body_t *magnet = make_magnet(scene, MAGNET_CENTER, false);

    rgb_color_t color = {.r = 0, .g = 0, .b = 0};
    vector_t *point2 = malloc(sizeof(vector_t));
    point2->x = 360; // remove magic numbers
    point2->y = 650;
    text_t *text2 = text_create("Back to Home", color, 22, point2);
    scene_add_text(scene, text2);

    // display a star and the amount of stars the user has
    char *stars = malloc(10*sizeof(char));
    sprintf(stars, "%i", get_star_count());
    vector_t *position = malloc(sizeof(vector_t));
    position->x = 110; // remove magic numbers
    position->y = 40;
    text_t *star_count = text_create(stars, color, 50, position);
    scene_add_text(scene, star_count);

    vector_t star_pos = {.x = 40, .y = SHOP_HEIGHT - 40};
    star_t *starframe = make_star(star_pos, 5, 25); // magic number for num points, radius
    rgb_color_t star_color = {.r = 1, .g = 1, .b = 0}; // make const for 'yellow' star color
    body_t *star = body_init(get_points(starframe), 0.001, star_color);
    scene_add_body(scene, star);

    body_t *background1 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = SHOP_HEIGHT});
    body_t *background2 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = 2*SHOP_HEIGHT});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);

    return scene;
}

void *shop_mouse_click() {
    // if immunity clicked
    buy_immunity();
    // else if magnet clicked
    buy_magnet();
    // else if boost clicked
    buy_boost();
}

int get_star_count() {
    FILE *star_file = fopen("stars.txt", "r+");
    if (star_file == NULL) {
        printf("NULL file.\n");
        return 1;
    }
    char *star_reading = malloc(10*sizeof(char));
    char **throwaway = malloc(sizeof(char *));
    *throwaway = malloc(10*sizeof(char));
    int num_stars;
    if (fgets(star_reading, 5, star_file) == NULL) {
        printf("Error.\n");
        return 1;
    }
    else {
        star_reading[6] = '\0';
        num_stars = (int)strtod(star_reading, throwaway);
    }
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
}

void write_powerup(char *powerup) {
    FILE *powerup_file = fopen("powerup.txt", "w");
    if (powerup_file == NULL) {
        printf("NULL file.\n");
    }
    fputs(powerup, powerup_file);
}

void buy_immunity() {
    int num_stars = get_star_count();
    if (num_stars < IMMUNITY_PRICE) {
        printf("Sorry, you don't have enough stars to purchase this.\n");
    }
    else {
        num_stars -= IMMUNITY_PRICE;
        change_star_count(num_stars);
        char *immunity = malloc(9*sizeof(char));
        immunity = "immunity";
        write_powerup(immunity);
    }
}

void buy_magnet(){
    int num_stars = get_star_count();
    if (num_stars < MAGNET_PRICE) {
        printf("Sorry, you don't have enough stars to purchase this.\n");
    }
    else {
        num_stars -= MAGNET_PRICE;
        change_star_count(num_stars);
        char *magnet = malloc(7*sizeof(char));
        magnet = "magnet";
        write_powerup(magnet);
    }
}

void buy_boost(){
    int num_stars = get_star_count();
    if (num_stars < BOOST_PRICE) {
        printf("Sorry, you don't have enough stars to purchase this.\n");
    }
    else {
        num_stars -= BOOST_PRICE;
        change_star_count(num_stars);
        char *boost = malloc(6*sizeof(char));
        boost = "boost";
        write_powerup(boost);
    }
}