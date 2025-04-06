#include <iostream>
#include <fstream>
#include <cmath>
#include <memory>

#include "rtweekend.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "cube.h"
#include "sphere.h"       // Usado para el piso (u otros objetos)
#include "shading.h"
#include "scene.h"        // Declara random_cubes_scene()

using std::make_shared;
using std::shared_ptr;

// Escribe el color en formato PPM.
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

// Función recursiva que calcula el color del rayo usando la dispersión del material.
color ray_color(const ray &r, const hittable &world, int depth) {
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }
    vec3 unit_direction = normalize(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1, 1, 1) + t * color(0.5, 0.7, 1);
}

int main() {
    // Parámetros de la imagen.
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    std::ofstream outfile("image.ppm");
    outfile << "P3\n" << image_width << " " << image_height << "\n255\n";

    // Construcción de la escena con cubos (definida en scene.cc).
    hittable_list world = random_cubes_scene();

    // Configuración de la cámara.
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    double vfov = 20.0;
    double aperture = 0.1;
    double focus_dist = 10.0;
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist);

    // Renderizado de la imagen.
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
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