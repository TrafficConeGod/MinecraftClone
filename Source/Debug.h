#include <iostream>
#include <chrono>

#define DEBUG_MODE true

void SubPrint();

template<typename T, typename ...Types>
void SubPrint(T firstArg, Types... args) {
    std::cout << firstArg << " ";
    SubPrint(args...);
}

template<typename ...Types>
void Print(Types... args) {
    #ifdef DEBUG_MODE
    SubPrint(args...);
    std::cout << "\n";
    #endif
}

class Timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
    public:
        Timer();

        int64_t Count() const;
};