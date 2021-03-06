#pragma once
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include "Math.h"

#define MK_ACROSS_OP(op) Vector3<T>& operator op##=(const Vector3<T>& vec) { \
    this->x op##= vec.x; \
    this->y op##= vec.y; \
    this->z op##= vec.z; \
    return *this; \
} \
friend Vector3<T> operator op (const Vector3<T> &vec1, const Vector3<T> &vec2) { \
    Vector3<T> vec(vec1.x, vec1.y, vec1.z); \
    vec op##= vec2; \
    return vec; \
} \

#define MK_SCALAR_OP(op) Vector3<T>& operator op##=(T scalar) { \
    this->x op##= scalar; \
    this->y op##= scalar; \
    this->z op##= scalar; \
    return *this; \
} \
friend Vector3<T> operator op (const Vector3<T> &vec1, T scalar) { \
    Vector3<T> vec(vec1.x, vec1.y, vec1.z); \
    vec op##= scalar; \
    return vec; \
} \

template<typename T>
struct Vector3 {
    T x;
    T y;
    T z;
    Vector3() : x{0}, y{0}, z{0} {}
    Vector3(T xVal, T yVal, T zVal) : x{xVal}, y{yVal}, z{zVal} {}
    friend std::ostream& operator<<(std::ostream& stream, const Vector3<T>& vec) {
        stream << vec.x << ", " << vec.y << ", " << vec.z;
        return stream;
    }
    MK_ACROSS_OP(+)
    MK_ACROSS_OP(-)
    MK_ACROSS_OP(*)
    MK_ACROSS_OP(/)
    MK_SCALAR_OP(*)
    MK_SCALAR_OP(/)
    double Magnitude() const {
        T a = x * x;
        T b = y * y;
        T c = z * z;
        double d = sqrt((double)(a + b + c));
        return d;
    }
    double SquareMagnitude() const {
        return Max(std::abs(x), std::abs(y), std::abs(z));
    }
    Vector3<T> Unit() const {
        Vector3<T> vec(x, y, z);
        T magnitude = Magnitude();
        return vec / magnitude;
    }
    T Dot(const Vector3<T> vector) const {
        return (x * vector.x) + (y * vector.y) + (z * vector.z);
    }
    inline glm::vec3 GLM() const {
        return glm::vec3(x, y, z);
    }
    template<typename T2>
    operator Vector3<T2>() const {
        return Vector3<T2>(x, y, z);
    }

    bool operator ==(const Vector3<T>& vec) {
        return x == vec.x && y == vec.y && z == vec.z;
    }
    bool operator !=(const Vector3<T>& vec) {
        return !(*this == vec);
    }
};

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector3u = Vector3<uint>;
using Vector3i = Vector3<int>;