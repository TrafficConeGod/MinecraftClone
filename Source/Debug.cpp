#include "Debug.h"

void SubPrint() {}

Timer::Timer() {
    #ifdef DEBUG_MODE
    start = std::chrono::high_resolution_clock::now();
    #endif
}

int64_t Timer::Count() const {
    #ifdef DEBUG_MODE
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    #else
    return 0;
    #endif
}