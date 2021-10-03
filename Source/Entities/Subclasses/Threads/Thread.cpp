#include "Thread.h"
#include "EntityReference.h"
#include <vector>
#include <mutex>
#include <iostream>

Thread::Thread() : Entity() {
    std::exception_ptr exceptionPtr = nullptr;

    worker = std::thread([&]() {
        try {
            Start();
            while (active) {
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