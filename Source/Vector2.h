#pragma once
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>

template<typename T>
struct Vector2 {
    T x;
    T y;
    Vector2() : x{0}, y{0} {}
    Vector2(T x_val, T y_val) : x{x_val}, y{y_val} {}
    friend std::ostream& operator<<(std::ostream& stream, const Vector2<T>& vec) {
        stream << vec.x << ", " << vec.y;
        return stream;
    }
    T Magnitude() const {
        T a = x * x;
        T b = y * y;
        T d = (T)sqrt((double)(a + b));
        return d;
    }
    Vector2<T> Normalize() const {
        Vector2<T> vec(x, y);
        T magnitude = 
        Magnitude();
        return vec / magnitude;
    }
    inline glm::vec2 GLM() const {
        return glm::vec2(x, y);
    }
};

typedef Vector2<float> Vector2f;
typedef Vector2<uint> Vector2u;
typedef Vector2<int> Vector2i;