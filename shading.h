#ifndef SHADING_H
#define SHADING_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"

float shadow_factor(const vec3 &p, const vec3 &n);
vec3 shade(const hit_record &rec);
vec3 ray_color(const ray &r);

#endif