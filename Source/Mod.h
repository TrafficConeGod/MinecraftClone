#pragma once

inline int Mod(int a, int b) {
    int ret = a % b;
    return ret >= 0 ? ret : ret + b;
}