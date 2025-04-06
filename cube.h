#ifndef CUBE_H
#define CUBE_H

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"

// La clase cube hereda de hittable y representa un cubo (AABB) definido
// por dos puntos: box_min y box_max. El material se almacena en mat_ptr.
class cube : public hittable {
public:
    point3 box_min;
    point3 box_max;
    shared_ptr<material> mat_ptr;

    cube() {}
    cube(const point3 &p0, const point3 &p1, shared_ptr<material> m)
        : box_min(p0), box_max(p1), mat_ptr(m) {}

    // Método para detectar la intersección con un rayo.
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    
    // Devuelve la caja envolvente del cubo.
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;
    
    virtual ~cube() = default;
};

// Función libre que interseca un cubo unitario definido en [-0.5, 0.5]^3 (sin material)
// Esto se usa, por ejemplo, para calcular sombras.
bool hit_cube(const ray &r, double t_min, double t_max, hit_record &rec);

#endif