#include "MainThread.h"
#include <iostream>

void MainThread::Update() {
    Capture(ConsoleOutput, [this]() {
        std::cout << "Hello!\n";
    });
}