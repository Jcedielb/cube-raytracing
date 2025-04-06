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


