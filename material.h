#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "ray.h"
#include "hittable.h"

struct hit_record;

class material {
public:
    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
    ) const = 0;
};

class lambertian : public material {
public:
    color albedo;
    lambertian(const color &a) : albedo(a) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
    ) const override {
        vec3 scatter_direction = rec.normal + normalize(vec3(random_double(), random_double(), random_double()));
        // Si scatter_direction es casi cero, usar rec.normal.
        if (fabs(scatter_direction.x()) < 1e-8 && fabs(scatter_direction.y()) < 1e-8 && fabs(scatter_direction.z()) < 1e-8)
            scatter_direction = rec.normal;
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

#endif