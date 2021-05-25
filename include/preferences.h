#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

bool get_sound_preference();

bool get_score_preference();

void switch_sound_preferences ();

void switch_score_preferences ();

void update_preferences();


#endif // #ifndef __PREFERENCES_H__