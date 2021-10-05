#include "Thread.h"
#include "EntityReference.h"
#include <vector>
#include <mutex>
#include <iostream>
#include <chrono>

static constexpr float Delta = 1.f/60.f;

std::mutex clockMutex;

Thread::Thread() : Entity() {
	auto clock = std::chrono::high_resolution_clock::now();

    std::exception_ptr exceptionPtr = nullptr;

    worker = std::thread([&]() {
        try {
            Start();
            while (active) {
                clockMutex.lock();
                auto currentClock = std::chrono::high_resolution_clock::now();
                clockMutex.unlock();

                std::chrono::duration<float> delta = currentClock - clock;
                float deltaTime = delta.count();
                if (delta.count() < Delta) {
                    deltaTime = 1/60;
                    float sleepTime = Delta - delta.count();
                    std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
                }

                clock = currentClock;
                Update(deltaTime);
            }
            Stop();
        } catch (...) {
            exceptionPtr = std::current_exception();
        }
    });

    if (exceptionPtr != nullptr) {
        try {
			std::rethrow_exception(exceptionPtr);
		} catch(const std::exception& exception) {
			std::cout << "Thread " << std::this_thread::get_id() << " caught exception from thread " << exception.what() << "\n";
		}
    }
}

Thread::~Thread() {
    if (!joined) {
        std::cout << "Thread was destructed without being joined\n";
    }
    active = false;
    Join();
}

void Thread::Join() {
    joined = true;
    worker.join();
}

void Thread::RequestStop() {
    active = false;
    RequestSubThreadsStop();
}