#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "ray.h"

class camera {
public:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

    camera(const point3 &lookfrom, const point3 &lookat, const vec3 &vup,
           double vfov, double aspect_ratio, double aperture, double focus_dist);

    ray get_ray(double s, double t) const;
};

#endif