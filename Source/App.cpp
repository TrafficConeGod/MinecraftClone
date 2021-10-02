#include "MainThread.h"
#include "EntityReference.h"
#include <iostream>

int main() {
    EntityReference<MainThread> mainThread = new MainThread();

    while (mainThread.Usages() > 0) {
        Thread::Capture(Thread::ConsoleOutput, [mainThread]() {
            std::cout << mainThread.Usages() << "\n";
        });
    }

    return 0;
}