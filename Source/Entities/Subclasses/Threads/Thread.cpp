#include "Thread.h"
#include "EntityReference.h"
#include <vector>
#include <mutex>
#include <iostream>
#include <chrono>

static const float Delta = 1.f/60.f;

Thread::Thread() : Entity() {
	auto clock = std::chrono::high_resolution_clock::now();

    std::exception_ptr exceptionPtr = nullptr;

    worker = std::thread([&]() {
        try {
            Start();
            while (active) {
                auto currentClock = std::chrono::high_resolution_clock::now();

                float delta = std::chrono::duration<float, std::ratio<1L, 1L>>(std::chrono::high_resolution_clock::now() - clock).count();
                std::cout << "tick\n";
                if (delta < Delta) {
                    float sleepTime = Delta - delta;
                    std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
                }

                clock = currentClock;
                Update();
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