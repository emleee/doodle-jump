#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "polygon.h"
#include "collision.h"
#include "body.h"
#include "test_util.h"
#include "color.h"

list_t *make_shape() {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {6, 6};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {8, 6};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {8, 8};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {6, 8};
    list_add(shape, v);
    return shape;
}

list_t *make_shape2() {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {0, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {+2, 0};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {+2, +2};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {0, +2};
    list_add(shape, v);
    return shape;
}

void test_collisions() {
    const double M = 10;
    body_t *square1 = body_init(make_shape(), M, (rgb_color_t) {0, 0, 0});
    body_t *square2 = body_init(make_shape2(), M, (rgb_color_t) {0, 0, 0});
    assert(!find_collision(body_get_shape(square1), body_get_shape(square2)).collided);
}

int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_collisions)

    puts(":) PASS");
}