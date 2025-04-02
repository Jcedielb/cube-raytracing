#ifndef CUBE_H
#define CUBE_H

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"

class cube : public hittable {
public:
    point3 box_min;
    point3 box_max;
    shared_ptr<material> mat_ptr;

    cube() {}
    cube(const point3 &p0, const point3 &p1, shared_ptr<material> m)
        : box_min(p0), box_max(p1), mat_ptr(m) {}

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;
    virtual ~cube() = default;
};

// Función libre para intersecar un cubo unitario en [-0.5,0.5]^3 sin material.
bool hit_cube(const ray &r, double t_min, double t_max, hit_record &rec);

#endif