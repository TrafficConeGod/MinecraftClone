#pragma once
#include <vector>
#include <functional>

// events should be for child -> parent communication ONLY.
template<typename... T>
class Event {
    private:
        std::mutex mutex;

        std::vector<std::function<void(T...)>> callbacks;
    public:
        void Listen(const std::function<void(T...)>& callback) { std::lock_guard<std::mutex> lock(mutex); callbacks.push_back(callback); }

        void Fire(const T&... args) { std::lock_guard<std::mutex> lock(mutex); for (const auto& callback : callbacks) { callback(args); } }
};