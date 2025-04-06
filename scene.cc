#include "scene.h"
#include "cube.h"
#include "sphere.h"
#include "material.h"
#include "rtweekend.h"
#include <memory>
#include <iostream>

using std::make_shared;
using std::shared_ptr;

hittable_list random_cubes_scene() {
    hittable_list world;

    double R = 150.0;
    auto floor_mat = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(
        point3(0, -R, 0),  
        R,                
        floor_mat
    ));

// Muchos cubos pequeños aleatorios
int cube_count = 0;
double separation = 1.5;  
double margin = 0.5;      

for (int a = -5; a <= 5; a++) {
    for (int b = -5; b <= 5; b++) {
        double choose_mat = random_double();
        point3 center(a * separation + 0.9 * random_double(), 0.2, b * separation + 0.9 * random_double());


        // Cubo grande 1 (vidrio): de (-1.5,0,-1.0) a (0.5,2.0,1.0)
        if (center.x() > (-1.5 - margin) && center.x() < (0.5 + margin) &&
            center.z() > (-1.0 - margin) && center.z() < (1.0 + margin))
            continue;
        // Cubo grande 2 (difuso): de (-5.0,0,-1.0) a (-3.0,2.0,1.0)
        if (center.x() > (-5.0 - margin) && center.x() < (-3.0 + margin) &&
            center.z() > (-1.0 - margin) && center.z() < (1.0 + margin))
            continue;
        // Cubo grande 3 (metálico): de (3.0,0,-1.0) a (5.0,2.0,1.0)
        if (center.x() > (3.0 - margin) && center.x() < (5.0 + margin) &&
            center.z() > (-1.0 - margin) && center.z() < (1.0 + margin))
            continue;

        shared_ptr<material> cube_mat;
        if (choose_mat < 0.4) {
            // Difuso
            auto albedo = color::random() * color::random();
            cube_mat = make_shared<lambertian>(albedo);
        } else if (choose_mat < 0.7) {
            // Metal
            auto albedo = color::random(0.5, 1);
            double fuzz = random_double() * 0.5;
            cube_mat = make_shared<metal>(albedo, fuzz);
        } else {
            // Vidrio
            cube_mat = make_shared<dielectric>(1.5);
        }

        double size = 0.2;
        point3 boxMin(center.x() - size, 0, center.z() - size);
        point3 boxMax(center.x() + size, 0.4, center.z() + size);
        world.add(make_shared<cube>(boxMin, boxMax, cube_mat));
        cube_count++;
    }
}
std::cerr << "Número de cubos pequeños generados: " << cube_count << "\n";

    // Cubo grande 1 (vidrio, reflectante)
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<cube>(
        point3(-1.5, 0, -1.0),
        point3(0.5, 2.0, 1.0),
        material1
    ));

    // Cubo grande 2 (difuso)
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<cube>(
        point3(-5.0, 0, -1.0),
        point3(-3.0, 2.0, 1.0),
        material2
    ));

    // Cubo grande 3 (metálico)
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<cube>(
        point3(3.0, 0, -1.0),
        point3(5.0, 2.0, 1.0),
        material3
    ));

    return world;
}
