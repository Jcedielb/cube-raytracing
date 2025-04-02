#include <iostream>
#include <fstream>
#include <cmath>
#include <memory>

#include "rtweekend.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "cube.h"
#include "sphere.h"      
#include "shading.h"

using std::make_shared;
using std::shared_ptr;

void write_color(std::ostream &out, const color &pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    double scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    int ir = static_cast<int>(256 * clamp(r, 0.0, 0.999));
    int ig = static_cast<int>(256 * clamp(g, 0.0, 0.999));
    int ib = static_cast<int>(256 * clamp(b, 0.0, 0.999));

    out << ir << " " << ig << " " << ib << "\n";
}

color ray_color(const ray &r, const hittable &world, int depth) {
    hit_record rec;
    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        // Llamamos a shade() para pintar el cubo de azul y la esfera de gris
        return shade(rec);
    }

    // Color de fondo (cielo degradado)
    vec3 unit_direction = normalize(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1,1,1) + t * color(0.5,0.7,1);
}

int main() {
    // Parámetros de la imagen
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width   = 800;
    const int image_height  = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 50;

    std::ofstream outfile("image.ppm");
    outfile << "P3\n" << image_width << " " << image_height << "\n255\n";

    // escena
    hittable_list world;

    // Piso curvo: esfera grande
    double R = 10.0;
    auto floor_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(
        point3(0, -R, 0),
        R,
        floor_material
    ));

    // Cubo azul: se define un cubo en [-0.5, 0, -0.5] hasta [0.5, 1, 0.5].
    auto cube_material = make_shared<lambertian>(color(1,1,1)); 
    world.add(make_shared<cube>(
        point3(-0.5, 0, -0.5),
        point3( 0.5, 1,  0.5),
        cube_material
    ));

    // Configuración de la cámara:
    point3 lookfrom(3, 2, 6); 
    point3 lookat(0, 0, 0);   
    vec3 vup(0, 1, 0);
    double vfov = 20.0;
    double aperture = 0.0;
    double focus_dist = 10.0;
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist);

    // Render
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (i + random_double()) / (image_width - 1);
                double v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(outfile, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
    outfile.close();
    return 0;
}