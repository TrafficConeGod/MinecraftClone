#include "Thread.h"
#include "EntityReference.h"
#include <vector>
#include <mutex>

Thread::Thread() {
    std::exception_ptr exceptionPtr = nullptr;

    worker = std::thread([&]() {
        try {
            while (true) {
                Update();
            }
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

void Thread::Join() {
    worker.join();
}

std::vector<std::mutex> mutexes(1);

void Thread::Capture(MutexId mutex, std::function<void()> context) {
    std::lock_guard<std::mutex> lock(mutexes.at(mutex));
    context();
}