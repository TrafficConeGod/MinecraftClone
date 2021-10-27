#pragma once

template<typename T>
inline T Mod(T a, T b) {
    T ret = a % b;
    return ret >= 0 ? ret : ret + b;
}

template<typename T>
inline double Max(T a, T b, T c) {
    return a > b ? (a > c ? a : c) : (b > c ? b : c);
}