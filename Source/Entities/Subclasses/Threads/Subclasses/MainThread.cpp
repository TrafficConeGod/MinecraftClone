#include "MainThread.h"
#include "EntityReference.h"
#include <iostream>

void MainThread::Update() {
    Capture(ConsoleOutput, [this]() {
        std::cout << x << "\n";
        x++;
    });
}