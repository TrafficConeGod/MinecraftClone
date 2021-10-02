#include "MainThread.h"
#include "EntityReference.h"
#include <iostream>

int main() {
    EntityReference<MainThread> mainThread = new MainThread();

    mainThread->Join();

    return 0;
}