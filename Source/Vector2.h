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
    template<typename T2>
    operator Vector2<T2>() const {
        return Vector2<T2>(x, y);
    }
};

using Vector2f = Vector2<float>;
using Vector2u = Vector2<uint>;
using Vector2i = Vector2<int>;