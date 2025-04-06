#ifndef VEC3_H
#define VEC3_H

// Declaración adelantada para poder usar random_double() en los métodos estáticos.
double random_double();

#include <cmath>

// Resto de la definición de vec3...
class vec3 {
public:
    double e[3];

    vec3() : e{0, 0, 0} {}
    vec3(double x, double y, double z) : e{x, y, z} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }
    // Método miembro para obtener la longitud del vector
    double length() const {
        return std::sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }

    // Métodos estáticos para generar un vector aleatorio.
    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }
    static vec3 random(double min, double max) {
        return vec3(random_double() * (max - min) + min,
                    random_double() * (max - min) + min,
                    random_double() * (max - min) + min);
    }
};

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t * v.x(), t * v.y(), t * v.z());
}
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}
inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}
inline double dot(const vec3 &u, const vec3 &v) {
    return u.x()*v.x() + u.y()*v.y() + u.z()*v.z();
}
inline double length(const vec3 &v) {
    return std::sqrt(dot(v, v));
}
inline vec3 normalize(const vec3 &v) {
    return v / length(v);
}
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.y()*v.z() - u.z()*v.y(),
                u.z()*v.x() - u.x()*v.z(),
                u.x()*v.y() - u.y()*v.x());
}

// Operador unario para invertir un vector.
inline vec3 operator-(const vec3 &v) {
    return vec3(-v.x(), -v.y(), -v.z());
}

// Sobrecarga del operador += para vec3
inline vec3& operator+=(vec3 &u, const vec3 &v) {
    u.e[0] += v.e[0];
    u.e[1] += v.e[1];
    u.e[2] += v.e[2];
    return u;
}

#endif