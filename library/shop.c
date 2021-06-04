#include "shop.h"
#include "color.h"
#include "preferences.h"

const vector_t BOOST_CENTER = {.x = 500, .y = 700};
const vector_t MAGNET_CENTER = {.x = 200, .y = 575};
const vector_t IMMUNITY_CENTER = {.x = 500, .y = 475};
const double PRICE_OFFSET = 60;
const double STAR_OFFSET = 60;
const vector_t SHOP_HOME_CENTER = {.x = 360, .y = 800};

const double SHOP_WIDTH = 720.0;
const double SHOP_HEIGHT = 960.0;

const int BOOST_PRICE = 100;
const int MAGNET_PRICE = 75;
const int IMMUNITY_PRICE = 50;
const rgb_color_t MESSAGE_COLOR = {.r = 0, .g = 0, .b = 0};
const rgb_color_t STAR_COLOR = {.r = 1, .g = 1, .b = 0};

// do something to check if there is something already in the inventory and give refund

scene_t *make_shop_scene () {
    char *scene_info = malloc(5*sizeof(char));
    strcpy(scene_info, "shop");
    scene_t *scene = scene_init_with_info(scene_info, free);
    // display the number of stars
    body_t *boost = make_boost(scene, BOOST_CENTER);

    body_t *immunity = make_immunity(scene, IMMUNITY_CENTER, false);
    body_t *magnet = make_magnet(scene, MAGNET_CENTER, false);

    rgb_color_t text_color = {.r = 0, .g = 0, .b = 0};

    vector_t *point1 = malloc(sizeof(vector_t));
    point1->x = BOOST_CENTER.x - 10; // remove magic numbers
    point1->y = SHOP_HEIGHT - BOOST_CENTER.y + PRICE_OFFSET;
    text_t *text1 = text_create("Buy for 50", text_color, 22, point1);
    scene_add_text(scene, text1);
    vector_t boost_star_center = {.x = BOOST_CENTER.x + STAR_OFFSET, .y = SHOP_HEIGHT - point1->y};
    star_t *boost_starframe = make_star(boost_star_center, 5, 13); // magic number for num points, radius
    char *boost_star_info = malloc(5*sizeof(char));
    strcpy(boost_star_info, "star");
    body_t *boost_star = body_init_with_info(get_points(boost_starframe), INFINITY, STAR_COLOR, boost_star_info, free);
    scene_add_body(scene, boost_star);

    vector_t *point2 = malloc(sizeof(vector_t));
    point2->x = MAGNET_CENTER.x - 10; // remove magic numbers
    point2->y = SHOP_HEIGHT - MAGNET_CENTER.y + PRICE_OFFSET * 2;
    text_t *text2 = text_create("Buy for 75", text_color, 22, point2);
    scene_add_text(scene, text2);
    vector_t magnet_star_center = {.x = MAGNET_CENTER.x + STAR_OFFSET, .y = SHOP_HEIGHT - point2->y};
    star_t *magnet_starframe = make_star(magnet_star_center, 5, 13); // magic number for num points, radius
    char *magnet_star_info = malloc(5*sizeof(char));
    strcpy(magnet_star_info, "star");
    body_t *magnet_star = body_init_with_info(get_points(magnet_starframe), INFINITY, STAR_COLOR, magnet_star_info, free);
    scene_add_body(scene, magnet_star);


    vector_t *point3 = malloc(sizeof(vector_t));
    point3->x = IMMUNITY_CENTER.x - 10; // remove magic numbers
    point3->y = SHOP_HEIGHT - IMMUNITY_CENTER.y + PRICE_OFFSET;
    text_t *text3 = text_create("Buy for 100", text_color, 22, point3);
    scene_add_text(scene, text3);
    vector_t immunity_star_center = {.x = IMMUNITY_CENTER.x + STAR_OFFSET + 5, .y = SHOP_HEIGHT - point3->y};
    star_t *immunity_starframe = make_star(immunity_star_center, 5, 13); // magic number for num points, radius
    char *immunity_star_info = malloc(5*sizeof(char));
    strcpy(immunity_star_info, "star");
    body_t *immunity_star = body_init_with_info(get_points(immunity_starframe), INFINITY, STAR_COLOR, immunity_star_info, free);
    scene_add_body(scene, immunity_star);

    vector_t *home_point = malloc(sizeof(vector_t));
    home_point->x = SHOP_HOME_CENTER.x; // remove magic numbers
    home_point->y = SHOP_HOME_CENTER.y;
    text_t *home_text = text_create("Back to Home", text_color, 22, home_point);
    scene_add_text(scene, home_text);

    display_star_count(scene);

    body_t *background1 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = SHOP_HEIGHT});
    body_t *background2 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = 2*SHOP_HEIGHT});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);

    return scene;
}

void display_star_count(scene_t *scene) {
    // display a star and the amount of stars the user has
    char *stars = malloc(10*sizeof(char));
    sprintf(stars, "%i", get_star_count());
    vector_t *position = malloc(sizeof(vector_t));
    position->x = 125; // remove magic numbers
    position->y = 40;
    text_t *star_count = text_create(stars, MESSAGE_COLOR, 50, position);
    scene_add_text(scene, star_count);

    vector_t star_pos = {.x = 40, .y = SHOP_HEIGHT - 40};
    star_t *starframe = make_star(star_pos, 5, 25); // magic number for num points, radius
    char *star_info = malloc(5*sizeof(char));
    strcpy(star_info, "star");
    body_t *star = body_init_with_info(get_points(starframe), 0.001, STAR_COLOR, star_info, free);
    scene_add_body(scene, star);
}

scene_t *make_shop_exit_scene() {
    char *scene_info = malloc(10*sizeof(char));
    strcpy(scene_info, "shop exit");
    scene_t *scene = scene_init_with_info(scene_info, free);
    vector_t center = {.x = SHOP_WIDTH/2, .y = SHOP_HEIGHT/2};
    list_t *shape = make_rectangle(center, 480, 237);
    char *info = malloc(sizeof(char)*5);
    strcpy(info, "exit");
    body_t *thanks = body_init_with_info(shape, INFINITY, MESSAGE_COLOR, info, free);
    sprite_t *sprite = create_sprite("PNGs/Shop_Exit.png", 480, 237);
    body_set_sprite(thanks, sprite);
    body_set_centroid(thanks, center);
    scene_add_body(scene, thanks);
    body_t *background1 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = SHOP_HEIGHT});
    body_t *background2 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = 2*SHOP_HEIGHT});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);

    return scene;
}

scene_t *make_failed_purchase_scene() {
    char *scene_info = malloc(10*sizeof(char));
    strcpy(scene_info, "shop fail");
    scene_t *scene = scene_init_with_info(scene_info, free);
    vector_t center = {.x = SHOP_WIDTH/2, .y = SHOP_HEIGHT/2};
    list_t *shape = make_rectangle(center, 480, 237);
    char *info = malloc(sizeof(char)*5);
    strcpy(info, "fail");
    body_t *thanks = body_init_with_info(shape, INFINITY, MESSAGE_COLOR, info, free);
    sprite_t *sprite = create_sprite("PNGs/Shop_Exit.png", 480, 237); //replace this with new sprite
    body_set_sprite(thanks, sprite);
    body_set_centroid(thanks, center);
    scene_add_body(scene, thanks);
    body_t *background1 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = SHOP_HEIGHT});
    body_t *background2 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = 2*SHOP_HEIGHT});
    scene_add_body(scene, background1);
    scene_add_body(scene, background2);

    return scene;
}

// scene_t *make_overwrite_boost_scene() {
//     char *scene_info = malloc(10*sizeof(char));
//     strcpy(scene_info, "shop fail");
//     scene_t *scene = scene_init_with_info(scene_info, free);
//     vector_t center = {.x = SHOP_WIDTH/2, .y = SHOP_HEIGHT/2};
//     list_t *shape = make_rectangle(center, 480, 237);
//     char *info = malloc(sizeof(char)*5);
//     strcpy(info, "fail");
//     body_t *thanks = body_init_with_info(shape, INFINITY, MESSAGE_COLOR, info, free);
//     sprite_t *sprite = create_sprite("PNGs/Shop_Exit.png", 480, 237); //replace this with new sprite
//     body_set_sprite(thanks, sprite);
//     body_set_centroid(thanks, center);
//     scene_add_body(scene, thanks);
//     body_t *background1 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = SHOP_HEIGHT});
//     body_t *background2 = make_background_body("PNGs/Game_Background.png",(vector_t){.x = 0, .y = 2*SHOP_HEIGHT});
//     scene_add_body(scene, background1);
//     scene_add_body(scene, background2);

//     return scene;
// }

void shop_mouse_click(scene_t *scene, int x, int y, double button_x_radius, double button_y_radius) {
    if (x < (220 + button_x_radius) && x > (220 - button_x_radius)) {
        if (y < (310 + button_y_radius) && y > (310 - button_y_radius)) {
            buy_boost(scene);
        }
    }
    if (x < (500 + button_x_radius) && x > (500 - button_x_radius)) {
        if (y < (450 + button_y_radius) && y > (450 - button_y_radius)) {
            buy_magnet(scene);
        }
    }
    if (x < (220 + button_x_radius) && x > (220 - button_x_radius)) {
        if (y < (550 + button_y_radius) && y > (550 - button_y_radius)) {
            buy_immunity(scene);
        }
    }
    if (x < (SHOP_HOME_CENTER.x + button_x_radius) && x > (SHOP_HOME_CENTER.x - button_x_radius)) {
        if (y < (SHOP_HOME_CENTER.y + button_y_radius) && y > (SHOP_HOME_CENTER.y - button_y_radius)) {
            char *start_info = malloc(6*sizeof(char));
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
    fclose(star_file);
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
        char *fail_info = malloc(10*sizeof(char));
        strcpy(fail_info, "shop fail");
        scene_set_next_info(scene, fail_info);
        printf("Sorry, you don't have enough stars to purchase the shield.\n");
    }
    else {
        if (get_sound_preference()) {
            play_cha_ching();
        }
        num_stars -= IMMUNITY_PRICE;
        change_star_count(num_stars);
        char *immunity = malloc(9*sizeof(char));
        immunity = "immunity";
        write_powerup(immunity);
        char *exit_info = malloc(10*sizeof(char));
        strcpy(exit_info, "shop exit");
        scene_set_next_info(scene, exit_info);
    }
}

void buy_magnet(scene_t *scene){
    int num_stars = get_star_count();
    if (num_stars < MAGNET_PRICE) {
        char *fail_info = malloc(10*sizeof(char));
        strcpy(fail_info, "shop fail");
        scene_set_next_info(scene, fail_info);
        printf("Sorry, you don't have enough stars to purchase the magnet.\n");
    }
    else {
        if (get_sound_preference()) {
            play_cha_ching();
        }
        num_stars -= MAGNET_PRICE;
        change_star_count(num_stars);
        char *magnet = malloc(7*sizeof(char));
        magnet = "magnet";
        write_powerup(magnet);
        char *exit_info = malloc(10*sizeof(char));
        strcpy(exit_info, "shop exit");
        scene_set_next_info(scene, exit_info);
    }
}

void buy_boost(scene_t *scene){
    int num_stars = get_star_count();
    if (num_stars < BOOST_PRICE) {
        char *fail_info = malloc(10*sizeof(char));
        strcpy(fail_info, "shop fail");
        scene_set_next_info(scene, fail_info);
        printf("Sorry, you don't have enough stars to purchase the boost.\n"); // display sorry scene
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
        char *exit_info = malloc(10*sizeof(char));
        strcpy(exit_info, "shop exit");
        scene_set_next_info(scene, exit_info);
    }
}

bool check_currency (int price) {
    int num_stars = get_star_count();
    if (num_stars < BOOST_PRICE) {
        return false;
    }
    return true;
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
            make_magnet(scene, center, false); //maybe false?
            fclose(file);
            file = fopen("inventory.txt", "w");
        }
        else if (strcmp(line, "immunity") == 0) {
            make_immunity(scene, center, false); //maybe false?
            fclose(file);
            file = fopen("inventory.txt", "w");
        }
    }
    // printf("sound is closed");
    fclose(file);
}