#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

scene_t *make_settings_scene();

bool get_sound_preference();

bool get_score_preference();

void switch_sound_preferences ();

void switch_score_preferences ();

void update_preferences();

void settings_mouse_click (scene_t *scene, int x, int y);

#endif // #ifndef __PREFERENCES_H__