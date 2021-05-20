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

body_t *make_platform(vector_t center, rgb_color_t color, char *info);

body_t *sliding_platform(vector_t center);

body_t *disappearing_platform(vector_t center);

body_t *trick_platform(vector_t center);