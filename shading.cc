#include "rtweekend.h"
#include "shading.h"
#include "cube.h"
#include "sphere.h"
#include <algorithm>
#include <cmath>

static const vec3 light_pos(2.f, 6.f, -2.f);

float shadow_factor(const vec3 &p, const vec3 &n) {
    vec3 to_light = normalize(light_pos - p);
    ray shadow_ray(p + 0.001f * n, to_light);
    hit_record tmp;
    if (hit_cube(shadow_ray, 0.001f, infinity, tmp))
        return 0.0f;
    return 1.0f;
}

vec3 shade(const hit_record &rec) {
    vec3 L = normalize(light_pos - rec.p);
    float diff = std::max(0.f, static_cast<float>(dot(rec.normal, L)));
    float sf = shadow_factor(rec.p, rec.normal);
    float ambient = 0.2f;
    float kd = 0.8f;
    // Tanto si se golpea el cubo como si se golpea la esfera se pinta de azul.
    return (ambient + kd * diff * sf) * vec3(0, 0, 1);
}

vec3 ray_color(const ray &r) {
    double R = 150.f;
    hit_record rec_cube, rec_ground;
    bool hitC = hit_cube(r, 0.001f, infinity, rec_cube);
    // Para el piso, usamos un objeto sphere temporal.
    sphere ground_sphere(point3(0, -R, 0), R, nullptr);
    bool hitG = ground_sphere.hit(r, 0.001f, infinity, rec_ground);

    if (!hitC && !hitG) {
        vec3 unit_dir = normalize(r.direction());
        double t = 0.5 * (unit_dir.y() + 1.0);
        return (1.0 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1);
    }
    double tC = hitC ? rec_cube.t : infinity;
    double tG = hitG ? rec_ground.t : infinity;
    if (tC < tG)
        return shade(rec_cube);
    else
        return shade(rec_ground);
}