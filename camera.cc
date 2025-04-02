#include "camera.h"

camera::camera(const point3 &lookfrom, const point3 &lookat, const vec3 &vup,
               double vfov, double aspect_ratio, double aperture, double focus_dist) {
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta/2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    origin = lookfrom;
    vec3 w = normalize(lookfrom - lookat);
    vec3 u = normalize(cross(vup, w));
    vec3 v = cross(w, u);

    lower_left_corner = origin - focus_dist * viewport_width/2 * u
                        - focus_dist * viewport_height/2 * v - focus_dist * w;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
}

ray camera::get_ray(double s, double t) const {
    return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
}