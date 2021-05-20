#ifndef __TEXT_H__
#define __TEXT_H__

#include "vector.h"
#include "color.h"
#include "SDL2\SDL.h"

/**
 * A textbox to display.
 * Includes an SDL_Surface, SDL_Texture, and SDL_Rect.
 */
typedef struct text text_t;

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
text_t *text_create(SDL_Renderer *renderer, char *string, rgb_color_t fontColor, char *fontFile, vector_t *center, double width, double height);

/**
 * Removes and frees textbox.
 *
 * @param text the textbox
 */
void text_free(text_t *text);

/**
 * Gets the surface of the text.
 * @param text the text_t to retrieve from
 * @return the surface
 */
SDL_Surface *text_get_surface(text_t *text);

/**
 * Gets the texture of the text.
 * @param text the text_t to retrieve from
 * @return the texture
 */
SDL_Texture *text_get_texture(text_t *text);

/**
 * Gets the textbox of the text.
 * @param text the text_t to retrieve from
 * @return the textbox
 */
SDL_Rect *get_textbox(text_t *text);

#endif // #ifndef __TEXT_H__