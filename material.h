#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "ray.h"
#include "hittable.h"
#include <memory>

using std::shared_ptr;

struct hit_record;  // Declaración adelantada

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
        vec3 scatter_direction = rec.normal + vec3::random();
        // Si scatter_direction es casi cero, usar la normal.
        if (fabs(scatter_direction.x()) < 1e-8 && fabs(scatter_direction.y()) < 1e-8 && fabs(scatter_direction.z()) < 1e-8)
            scatter_direction = rec.normal;
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class metal : public material {
    public:
        color albedo;
        double fuzz;
        metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    
        virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
        ) const override {
            // Normaliza la dirección del rayo entrante
            vec3 unit_direction = normalize(r_in.direction());
            // Calculam la reflexión
            vec3 reflected = reflect(unit_direction, rec.normal);
            // Suma el ruido (fuzz) usando random_in_unit_sphere()
            vec3 scattered_direction = reflected + fuzz * random_in_unit_sphere();
            scattered = ray(rec.p, scattered_direction);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    };

    class dielectric : public material {
        public:
            double ir; // Índice de refracción
            dielectric(double index_of_refraction) : ir(index_of_refraction) {}
        
            virtual bool scatter(
                const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
            ) const override {
                attenuation = color(1.0, 1.0, 1.0);
                double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;
                vec3 unit_direction = normalize(r_in.direction());
                double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
                double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        
                auto reflectance = [](double cosine, double ref_idx) -> double {
                    // Aproximación de Schlick
                    auto r0 = (1 - ref_idx) / (1 + ref_idx);
                    r0 = r0 * r0;
                    return r0 + (1 - r0) * pow((1 - cosine), 5);
                };
        
                bool cannot_refract = refraction_ratio * sin_theta > 1.0;
                vec3 direction;
                if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                    direction = reflect(unit_direction, rec.normal);
                else
                    direction = refract(unit_direction, rec.normal, refraction_ratio);
        
                scattered = ray(rec.p, direction);
                return true;
            }
        };
#endif