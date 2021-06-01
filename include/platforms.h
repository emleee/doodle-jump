#include "polygon.h"
#include "forces.h"
#include "collision.h"

body_t *make_platform(vector_t center, rgb_color_t color, char *info, int width, int height, sprite_t *sprite);

body_t *normal_platform(vector_t center, char *info);

body_t *sliding_platform(vector_t center, char *info);

void sliding_bounce(body_t *body);

body_t *disappearing_platform(vector_t center, char *info);

body_t *trick_platform(vector_t center, char *info);