#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mouse.h>
#include "sdl_wrapper.h"
#include "list.h"
#include "body.h"

const char WINDOW_TITLE[] = "Doodle Jump 3: Fairy Tail";
const int WINDOW_WIDTH = 720;
const int WINDOW_HEIGHT = 960;
const double MS_PER_S = 1e3;

/**
 * The coordinate at the center of the screen.
 */
vector_t center;
/**
 * The coordinate difference from the center to the top right corner.
 */
vector_t max_diff;
/**
 * The SDL window where the scene is rendered.
 */
SDL_Window *window;
/**
 * The renderer used to draw the scene.
 */
SDL_Renderer *renderer;
/**
 * The keypress handler, or NULL if none has been configured.
 */
key_handler_t key_handler = NULL;
/**
 * SDL's timestamp when a key was last pressed or released.
 * Used to mesasure how long a key has been held.
 */
uint32_t key_start_timestamp;

mouse_handler_t mouse_handler = NULL;

/**
 * The value of clock() when time_since_last_tick() was last called.
 * Initially 0.
 */
clock_t last_clock = 0;

//The sound effects that will be used
Mix_Chunk *jump = NULL;
Mix_Chunk *shoot = NULL;
Mix_Chunk *cha_ching = NULL;

/** Computes the center of the window in pixel coordinates */
vector_t get_window_center(void) {
    int *width = malloc(sizeof(*width)),
        *height = malloc(sizeof(*height));
    assert(width != NULL);
    assert(height != NULL);
    SDL_GetWindowSize(window, width, height);
    vector_t dimensions = {.x = *width, .y = *height};
    free(width);
    free(height);
    return vec_multiply(0.5, dimensions);
    // return center;
}

void sdl_set_center(vector_t new_center) {
    center = new_center;
}

/**
 * Computes the scaling factor between scene coordinates and pixel coordinates.
 * The scene is scaled by the same factor in the x and y dimensions,
 * chosen to maximize the size of the scene while keeping it in the window.
 */
double get_scene_scale(vector_t window_center) {
    // Scale scene so it fits entirely in the window
    double x_scale = window_center.x / max_diff.x,
           y_scale = window_center.y / max_diff.y;
    return x_scale < y_scale ? x_scale : y_scale;
}

/** Maps a scene coordinate to a window coordinate */
vector_t get_window_position(vector_t scene_pos, vector_t window_center) {
    // Scale scene coordinates by the scaling factor
    // and map the center of the scene to the center of the window
    vector_t scene_center_offset = vec_subtract(scene_pos, center);
    double scale = get_scene_scale(window_center);
    vector_t pixel_center_offset = vec_multiply(scale, scene_center_offset);
    vector_t pixel = {
        .x = round(window_center.x + pixel_center_offset.x),
        // Flip y axis since positive y is down on the screen
        .y = round(window_center.y - pixel_center_offset.y)
    };
    return pixel;
}

/**
 * Converts an SDL key code to a char.
 * 7-bit ASCII characters are just returned
 * and arrow keys are given special character codes.
 */
char get_keycode(SDL_Keycode key) {
    switch (key) {
        case SDLK_a:  return LEFT_ARROW;
        // case SDLK_UP:    return UP_ARROW;
        case SDLK_d: return RIGHT_ARROW;
        // case SDLK_DOWN:  return DOWN_ARROW;
        default:
            // Only process 7-bit ASCII characters
            return key == (SDL_Keycode) (char) key ? key : '\0';
    }
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sound effects
	jump = Mix_LoadWAV( "sound_effects/jump.wav" );
	if( jump == NULL )
	{
		printf( "Failed to load jumping sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	shoot = Mix_LoadWAV( "sound_effects/shoot.wav" );
	if( shoot == NULL )
	{
		printf( "Failed to load shooting sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

    cha_ching = Mix_LoadWAV( "sound_effects/cha_ching.wav" );
	if( cha_ching == NULL )
	{
		printf( "Failed to load cash register sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	return success;
}

void play_jump() {
    if (loadMedia()) {
        Mix_PlayChannel( -1, jump, 0 );
    }
}

void play_shoot() {
    if (loadMedia()) {
        Mix_PlayChannel( -1, shoot, 0 );
    }
}

void play_cha_ching() {
    if (loadMedia()) {
        Mix_PlayChannel( -1, cha_ching, 0 );
    }
}


void sdl_init(vector_t min, vector_t max) {
    // Check parameters
    assert(min.x < max.x);
    assert(min.y < max.y);

    center = vec_multiply(0.5, vec_add(min, max));
    max_diff = vec_subtract(max, center);
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    renderer = SDL_CreateRenderer(window, -1, 0);

    // //Initialize SDL_mixer
	if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	}
}

sprite_t *create_sprite(char *file, int width, int height) {
    SDL_Surface *image = IMG_Load(file);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    sprite_t *sprite = sprite_init(texture, width, height);
    SDL_FreeSurface(image);
    return sprite;
}


text_t *text_create(char *string, rgb_color_t fontColor, int ptsize, vector_t *center) {
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("fonts/Nunito-SemiBold.ttf", ptsize);
    SDL_Color color = {fontColor.r, fontColor.g, fontColor.b};
    SDL_Surface *surface = TTF_RenderText_Solid(font, string, color);
    // SDL_Texture *texture = malloc(sizeof(SDL_Texture));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect *textbox = malloc(sizeof(SDL_Rect));
    textbox->x = center->x - (surface->w / 2);
    textbox->y = center->y - (surface->h / 2);
    textbox->w = surface->w;
    textbox->h = surface->h;
    free(center);
    
    text_t *text = text_init(surface, texture, textbox);
    // SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    TTF_Quit();
    return text;
}

bool sdl_is_done(void *scene) {
    SDL_Event *event = malloc(sizeof(*event));
    int *x;
    int *y;
    assert(event != NULL);
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                free(event);
                return true;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                // Skip the keypress if no handler is configured
                // or an unrecognized key was pressed
                if (key_handler == NULL) break;
                char key = get_keycode(event->key.keysym.sym);
                if (key == '\0') break;

                uint32_t timestamp = event->key.timestamp;
                if (!event->key.repeat) {
                    key_start_timestamp = timestamp;
                }
                key_event_type_t type =
                    event->type == SDL_KEYDOWN ? KEY_PRESSED : KEY_RELEASED;
                double held_time = (timestamp - key_start_timestamp) / MS_PER_S;
                key_handler(key, type, held_time, scene);
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = malloc(sizeof(int));
                y = malloc(sizeof(int));
                SDL_GetMouseState(x, y);
                mouse_handler(event->button.button, *x, *y, scene);
                free(x);
                free(y);
                break;
        }
    }
    free(event);
    return false;
}

void sdl_clear(void) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void sdl_draw_polygon(list_t *points, rgb_color_t color) {
    // Check parameters
    size_t n = list_size(points);
    assert(n >= 3);
    assert(0 <= color.r && color.r <= 1);
    assert(0 <= color.g && color.g <= 1);
    assert(0 <= color.b && color.b <= 1);

    vector_t window_center = get_window_center();

    // Convert each vertex to a point on screen
    int16_t *x_points = malloc(sizeof(*x_points) * n),
            *y_points = malloc(sizeof(*y_points) * n);
    assert(x_points != NULL);
    assert(y_points != NULL);
    for (size_t i = 0; i < n; i++) {
        vector_t *vertex = list_get(points, i);
        vector_t pixel = get_window_position(*vertex, window_center);
        x_points[i] = pixel.x;
        y_points[i] = pixel.y;
    }

    // Draw polygon with the given color
    filledPolygonRGBA(
        renderer,
        x_points, y_points, n,
        color.r * 255, color.g * 255, color.b * 255, 255
    );
    free(x_points);
    free(y_points);
    list_free(points);
}

void sdl_show(scene_t *scene) {
    // Draw boundary lines
    vector_t window_center = get_window_center();
    vector_t max = vec_add(center, max_diff),
             min = vec_subtract(center, max_diff);
    vector_t max_pixel = get_window_position(max, window_center),
             min_pixel = get_window_position(min, window_center);
    SDL_Rect *boundary = malloc(sizeof(*boundary));
    boundary->x = min_pixel.x;
    boundary->y = max_pixel.y;
    boundary->w = max_pixel.x - min_pixel.x;
    boundary->h = min_pixel.y - max_pixel.y;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, boundary);
    free(boundary);

    for(int i = 0; i < scene_bodies(scene); i++) {
        sprite_t *sprite = body_get_sprite(scene_get_body(scene, i));
        if (sprite!=NULL && strcmp(body_get_info(scene_get_body(scene, i)), "background") == 0) {
            SDL_RenderCopy(renderer, sprite_get_texture(sprite), NULL, sprite_get_box(sprite));
        }
    }

    for(int i = 0; i < scene_bodies(scene); i++) {
        sprite_t *sprite = body_get_sprite(scene_get_body(scene, i));
        if (sprite!=NULL && strcmp(body_get_info(scene_get_body(scene, i)), "background") != 0) {
            SDL_RenderCopy(renderer, sprite_get_texture(sprite), NULL, sprite_get_box(sprite));
        }
    }

    size_t body_count = scene_bodies(scene);
    for (size_t i = 0; i < body_count; i++) {
        body_t *body = scene_get_body(scene, i);
        if (body_get_sprite(body) == NULL) {
            sdl_draw_polygon(body_get_shape(body), body_get_color(body));
        }
    }

    // go through and render all the text in the scene
    for (size_t i = 0; i < scene_textboxes(scene); i++) {
        text_t *current = scene_get_text(scene, i);
        SDL_Texture *texture = text_get_texture(current);
        SDL_Rect *textbox = text_get_textbox(current);
        SDL_RenderCopy(renderer, texture, NULL, textbox);
    }
    SDL_RenderPresent(renderer);
}

void sdl_render_scene(scene_t *scene) {
    sdl_clear();
    sdl_show(scene);
}

void sdl_on_key(key_handler_t handler) {
    key_handler = handler;
}

void sdl_mouse(mouse_handler_t handler) {
    mouse_handler = handler;
}

double time_since_last_tick(void) {
    clock_t now = clock();
    double difference = last_clock
        ? (double) (now - last_clock) / CLOCKS_PER_SEC
        : 0.0; // return 0 the first time this is called
    last_clock = now;
    return difference;
}

void reset_clock(void) {
    last_clock = 0;
}

SDL_Renderer *get_renderer(void) {
    return renderer;
}

void free_text(text_t *text) {
    SDL_FreeSurface(text_get_surface(text));
    SDL_DestroyTexture(text_get_texture(text));
    free(text);
}

void free_sounds() {
    //Quit SDL subsystems
    if (jump != NULL) {
        Mix_FreeChunk(jump);
        jump = NULL;
    }
    if (shoot != NULL) {
        Mix_FreeChunk(shoot);
        shoot = NULL;
    }
    if (cha_ching != NULL) {
        Mix_FreeChunk(cha_ching);
        cha_ching = NULL;
    }
    Mix_Quit();
}
