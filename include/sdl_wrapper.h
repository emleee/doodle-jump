#ifndef __SDL_WRAPPER_H__
#define __SDL_WRAPPER_H__

#include <stdbool.h>
#include "scene.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "list.h"

// Values passed to a key handler when the given arrow key is pressed
typedef enum {
    LEFT_ARROW = 1,
    UP_ARROW = 2,
    RIGHT_ARROW = 3,
    DOWN_ARROW = 4,
    SPACE = ' '
} arrow_key_t;

/**
 * The possible types of key events.
 * Enum types in C are much more primitive than in Java; this is equivalent to:
 * typedef unsigned int KeyEventType;
 * #define KEY_PRESSED 0
 * #define KEY_RELEASED 1
 */
typedef enum {
    KEY_PRESSED,
    KEY_RELEASED
} key_event_type_t;

/**
 * A keypress handler.
 * When a key is pressed or released, the handler is passed its char value.
 * The 'a' and 'd' keys function as left and right arrows respectively.
 *
 * @param key a character indicating which key was pressed
 * @param type the type of key event (KEY_PRESSED or KEY_RELEASED)
 * @param held_time if a press event, the time the key has been held in seconds
 */
typedef void (*key_handler_t)(char key, key_event_type_t type, double held_time, void *scene);

/**
 * A mouseclick handler.
 * When the mouse is left-clicked, the handler is passed the click coordinate.
 *
 * @param key an integer indicating which key was pressed
 * @param x the x coordinate of the click
 * @param y the y coordinate of the click
 * @param scene the pointer to the scene where the clicking occurs
 */
typedef void (*mouse_handler_t)(int key, int x, int y, void *scene);

/**
 * Loads sound-effects for jumping, shooting, and purchasing powerups.
 *
 * @return whether or not the loading for each of the sound-effects was successful
 */
bool loadMedia();

/**
 * Plays the jumping sound-effect.
 */
void play_jump();

/**
 * Plays the shooting sound-effect.
 */
void play_shoot();

/**
 * Plays the cash register sound-effect.
 */
void play_cha_ching();

/**
 * Closes the mixer and frees all sound-effect variables.
 */
void free_sounds();

/**
 * Initializes the SDL window, renderer, and mixer.
 * Must be called once before any of the other SDL functions.
 *
 * @param min the x and y coordinates of the bottom left of the scene
 * @param max the x and y coordinates of the top right of the scene
 */
void sdl_init(vector_t min, vector_t max);

/**
 * Computes and returns the coordinates of the center of the window
 * @return a vector_t representing the coordinates of the window's center
 */
vector_t get_window_center(void);

/**
 * Sets the center of the window to the given center
 * @param new_center the coordinates of the new center
 */
void sdl_set_center(vector_t new_center);

/**
 * Creates and renders the a sprite from a file.
 * 
 * @param file the file where the sprite's image is found
 * @param width the width of the sprite
 * @param height the height of the sprite
 * @return a newly allocated sprite_t 
 */
sprite_t *create_sprite(char *file, int width, int height);

/**
 * Processes all SDL events and returns whether the window has been closed.
 * This function must be called in order to handle keypresses.
 *
 * @return true if the window was closed, false otherwise
 */
bool sdl_is_done(void * scene);

/**
 * Clears the screen. Should be called before drawing polygons in each frame.
 */
void sdl_clear(void);

/**
 * Draws a polygon from the given list of vertices and a color.
 *
 * @param points the list of vertices of the polygon
 * @param color the color used to fill in the polygon
 */
void sdl_draw_polygon(list_t *points, rgb_color_t color);

/**
 * Displays the rendered frame on the SDL window.
 * Must be called after drawing the polygons in order to show them.
 */
void sdl_show(scene_t *scene);

/**
 * Draws all bodies in a scene.
 * This internally calls sdl_clear(), sdl_draw_polygon(), and sdl_show(),
 * so those functions should not be called directly.
 *
 * @param scene the scene to draw
 */
void sdl_render_scene(scene_t *scene);

/**
 * Registers a function to be called every time a key is pressed.
 * Overwrites any existing handler.
 *
 * Example:
 * ```
 * void on_key(char key, key_event_type_t type, double held_time) {
 *     if (type == KEY_PRESSED) {
 *         switch (key) {
 *             case 'a':
 *                 printf("A pressed\n");
 *                 break;
 *             case UP_ARROW:
 *                 printf("UP pressed\n");
 *                 break;
 *         }
 *     }
 * }
 * int main(void) {
 *     sdl_on_key(on_key);
 *     while (!sdl_is_done());
 * }
 * ```
 *
 * @param handler the function to call with each key press
 */
void sdl_on_key(key_handler_t handler);

/**
 * Registers a function to be called every time the mouse is clicked.
 * 
 * @param handler the function to call with every mouse click
 */
void sdl_mouse(mouse_handler_t handler);

/**
 * Gets the amount of time that has passed since the last time
 * this function was called, in seconds.
 *
 * @return the number of seconds that have elapsed
 */
double time_since_last_tick(void);

/**
 * Sets the clock back to 0.
 */
void reset_clock(void);

/**
 * Returns the renderer to use with text implementation.
 *
 * @return the renderer
 */
SDL_Renderer *get_renderer(void);

/**
 * Converts and returns scene coordinates to window coordinates.
 * 
 * @param scene_pos the position of the object in the scene's coordinates
 * @param window_center the window's center in scene coordinates
 */
vector_t get_window_position(vector_t scene_pos, vector_t window_center);

/**
 * Creates a textbox for the string based on given font, color, and positioning.
 *
 * @param string the string to display
 * @param fontColor the color of text
 * @param fontFile file containing font desired
 * @param center center of textbox
 * @param width width of textbox
 * @param height height of textbox
 *
 * @return text struct containing surface, texture, textbox rectangle
 */
text_t *text_create(char *string, rgb_color_t fontColor, int ptsize, vector_t *center);


#endif // #ifndef __SDL_WRAPPER_H__
