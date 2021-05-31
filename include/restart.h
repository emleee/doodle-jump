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

void restart_mouse_click(scene_t *scene, int x, int y, double button_x_radius, double button_y_radius);
scene_t *make_restart_scene(char *score);