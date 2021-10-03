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
        Event() {}
        Event(const Event<T...>& event) : callbacks{event.callbacks} {}
        Event(const std::function<void(T...)>& callback) { Listen(callback); }

        void Listen(const std::function<void(T...)>& callback) { std::lock_guard<std::mutex> lock(mutex); callbacks.push_back(callback); }

        void Fire(const T&... args) { std::lock_guard<std::mutex> lock(mutex); for (const auto& callback : callbacks) { callback(args...); } }
};