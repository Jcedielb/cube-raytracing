#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <random>
#include "vec3.h"

using point3 = vec3;
using color  = vec3;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Convierte grados a radianes.
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Función clamp: restringe x entre min y max.
inline double clamp(double x, double min_val, double max_val) {
    if (x < min_val) return min_val;
    if (x > max_val) return max_val;
    return x;
}

// Generador de números aleatorios en [0,1)
static std::mt19937 rng(1234); // Semilla fija
static std::uniform_real_distribution<double> dist(0.0, 1.0);
inline double random_double() {
    return dist(rng);
}
inline double random_float() { // alias
    return random_double();
}


// Clase aabb: caja envolvente alineada a los ejes.
class aabb {
    public:
        point3 minimum;
        point3 maximum;
    
        aabb() {}
        aabb(const point3& a, const point3& b) : minimum(a), maximum(b) {}
    
        point3 min() const { return minimum; }
        point3 max() const { return maximum; }
    };
    
    // Función auxiliar para generar la caja envolvente entre dos aabb.
    inline aabb surrounding_box(aabb box0, aabb box1) {
        point3 small(fmin(box0.min().x(), box1.min().x()),
                     fmin(box0.min().y(), box1.min().y()),
                     fmin(box0.min().z(), box1.min().z()));
        point3 big(fmax(box0.max().x(), box1.max().x()),
                   fmax(box0.max().y(), box1.max().y()),
                   fmax(box0.max().z(), box1.max().z()));
        return aabb(small, big);
    }
#endif