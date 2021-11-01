#include "Thread.h"
#include "EntityReference.h"
#include <vector>
#include <mutex>
#include <iostream>
#include <chrono>
#include "Debug.h"

static constexpr float Delta = 1.f/60.f;

Thread::Thread() : worker{
    [&]() {
        auto clock = std::chrono::high_resolution_clock::now();
        Start();
        while (active) {
            auto currentClock = std::chrono::high_resolution_clock::now();

            std::chrono::duration<float> delta = currentClock - clock;
            float deltaTime = delta.count();
            if (delta.count() < Delta) {
                deltaTime = 1/60;
                float sleepTime = Delta - delta.count();
                std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
            }

            Update(deltaTime);
            clock = currentClock;
        }
        Stop();
    }
} {}

Thread::~Thread() {
    if (!joined) {
        Print("Thread was destructed without being joined\n");
        active = false;
        Join();
    }
}

void Thread::Join() {
    joined = true;
    worker.join();
}

void Thread::RequestStop() {
    active = false;
    RequestSubThreadsStop();
}