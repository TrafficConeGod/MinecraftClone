#include "Thread.h"
#include "EntityReference.h"
#include <vector>
#include <mutex>

Thread::Thread() {
    EntityReference<Thread> reference = this;
    worker = std::thread([](EntityReference<Thread> reference) {
        while (reference.Usages() > 0) {
            reference->Update();
        }
    }, reference);
}

std::vector<std::mutex> mutexes(1);

void Thread::Capture(MutexId mutex, std::function<void()> context) {
    std::lock_guard<std::mutex> lock(mutexes.at(mutex));
    context();
}