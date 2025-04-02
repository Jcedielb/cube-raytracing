#include "cube.h"
#include <algorithm>
#include <cmath>

bool cube::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    double t0 = t_min;
    double t1 = t_max;
    for (int i = 0; i < 3; i++) {
        double invD = 1.0 / r.direction().e[i];
        double tNear = (box_min.e[i] - r.origin().e[i]) * invD;
        double tFar  = (box_max.e[i] - r.origin().e[i]) * invD;
        if (invD < 0.0) std::swap(tNear, tFar);
        if (tNear > t0) t0 = tNear;
        if (tFar  < t1) t1 = tFar;
        if (t1 <= t0)
            return false;
    }
    rec.t = t0;
    rec.p = r.at(t0);
    double bias = 1e-4;
    vec3 outward_normal;
    if (std::fabs(rec.p.e[0] - box_max.e[0]) < bias)
        outward_normal = vec3(1,0,0);
    else if (std::fabs(rec.p.e[0] - box_min.e[0]) < bias)
        outward_normal = vec3(-1,0,0);
    else if (std::fabs(rec.p.e[1] - box_max.e[1]) < bias)
        outward_normal = vec3(0,1,0);
    else if (std::fabs(rec.p.e[1] - box_min.e[1]) < bias)
        outward_normal = vec3(0,-1,0);
    else if (std::fabs(rec.p.e[2] - box_max.e[2]) < bias)
        outward_normal = vec3(0,0,1);
    else if (std::fabs(rec.p.e[2] - box_min.e[2]) < bias)
        outward_normal = vec3(0,0,-1);
    else
        outward_normal = vec3(0,1,0);
    rec.set_face_normal(r, outward_normal);
    rec.hit_cube = true;
    rec.mat_ptr = mat_ptr;
    return true;
}

bool cube::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = aabb(box_min, box_max);
    return true;
}

bool hit_cube(const ray &r, double t_min, double t_max, hit_record &rec) {
    cube unit_cube(point3(-0.5, -0.5, -0.5), point3(0.5, 0.5, 0.5), nullptr);
    return unit_cube.hit(r, t_min, t_max, rec);
}