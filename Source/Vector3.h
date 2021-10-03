#pragma once
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>

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
    T Magnitude() const {
        T a = x * x;
        T b = y * y;
        T c = z * z;
        T d = (T)sqrt((double)(a + b + c));
        return d;
    }
    Vector3<T> Normalize() const {
        Vector3<T> vec(x, y, z);
        T magnitude = Magnitude();
        return vec / magnitude;
    }
    inline glm::vec3 GLM() const {
        return glm::vec3(x, y, z);
    }
};

typedef Vector3<float> Vector3f;
typedef Vector3<uint> Vector3u;
typedef Vector3<int> Vector3i;