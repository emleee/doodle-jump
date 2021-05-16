#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "forces.h"
#include "test_util.h"

list_t *make_shape() {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t){-1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){+1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){+1, +1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){-1, +1};
    list_add(shape, v);
    return shape;
}

double kinetic_energy(body_t *body) {
    vector_t v = body_get_velocity(body);
    return body_get_mass(body) * vec_dot(v, v) / 2;
}

double spring_potential(body_t *mass, body_t *anchor, double k) {
    vector_t dx = vec_subtract(body_get_centroid(mass), body_get_centroid(anchor));
    return k * vec_dot(dx, dx) / 2;
}

// Tests that an oscillating spring conserves (kinetic energy + spring potential energy)
void test_spring_energy() {
    const double M = 10;
    const double A = 3;
    const double K = 2;
    const double DT = 1e-6;
    const int STEPS = 1000000;

    scene_t *scene = scene_init();
    body_t *mass = body_init(make_shape(), M, (rgb_color_t){0, 0, 0});
    body_set_centroid(mass, (vector_t){A, 0});
    scene_add_body(scene, mass);
    body_t *anchor = body_init(make_shape(), INFINITY, (rgb_color_t){0, 0, 0});
    scene_add_body(scene, anchor);

    create_spring(scene, K, mass, anchor);
    double initial_energy = spring_potential(mass, anchor, K);

    for (int i = 0; i < STEPS; i++) {
        double curr_energy = spring_potential(mass, anchor, K) + kinetic_energy(mass);
        assert(within(1e-4,curr_energy, initial_energy));
        assert(vec_equal(body_get_centroid(anchor), VEC_ZERO));

        scene_tick(scene, DT);
    }

    scene_free(scene);
}

// Tests that an object experiencing a drag force has the expected displacement
void test_drag() {
    const double M = 10;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    const double GAMMA = 0.3;

    scene_t *scene = scene_init();
    body_t *mass = body_init(make_shape(), M, (rgb_color_t){0, 0, 0});
    body_set_velocity(mass, (vector_t){20,30});
    scene_add_body(scene, mass);

    create_drag(scene, GAMMA, mass);
    vector_t prev_velocity = body_get_velocity(mass);
    vector_t dx = VEC_ZERO;

    for (int i = 0; i < STEPS; i++) {
        scene_tick(scene, DT);
        
        assert(body_get_velocity(mass).x < prev_velocity.x);
        assert(body_get_velocity(mass).y < prev_velocity.y);
        vector_t acceleration = vec_multiply(1/body_get_mass(mass), vec_multiply(GAMMA, vec_negate(prev_velocity)));
        dx = vec_add(dx, vec_subtract(vec_multiply(DT, body_get_velocity(mass)), vec_multiply(pow(DT, 2)/2, acceleration)));
        assert(vec_isclose(body_get_centroid(mass), dx));
        prev_velocity = body_get_velocity(mass);
    }

    scene_free(scene);
}

vector_t momentum(body_t *mass) {
    return vec_multiply(body_get_mass(mass), body_get_velocity(mass));
}

// Tests that momentum of the system is conserved at all times
void test_gravity() {
    const double M1 = 4.5, M2 = 7.3;
    const double G = 1e3;
    const double DT = 1e-6;
    const int STEPS = 1000000;

    scene_t *scene = scene_init();
    body_t *mass1 = body_init(make_shape(), M1, (rgb_color_t){0, 0, 0});
    scene_add_body(scene, mass1);
    body_t *mass2 = body_init(make_shape(), M2, (rgb_color_t){0, 0, 0});
    body_set_centroid(mass2, (vector_t){10, 20});
    scene_add_body(scene, mass2);

    create_newtonian_gravity(scene, G, mass1, mass2);

    for (int i = 0; i < STEPS; i++) {
        assert(body_get_centroid(mass1).x < body_get_centroid(mass2).x);
        vector_t p = vec_add(momentum(mass1), momentum(mass2));
        assert(vec_isclose(p, VEC_ZERO));

        scene_tick(scene, DT);
    }

    scene_free(scene);
}

int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;

    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_gravity)
    DO_TEST(test_spring_energy)
    DO_TEST(test_drag)

    puts("student_tests PASS");
}
