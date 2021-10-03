#pragma once
#include <vector>
#include <functional>

template<typename... T>
class Event {
    private:
        std::vector<std::function<void(T...)>> callbacks;
    public:
        void Listen(const std::function<void(T...)>& callback) { callbacks.push_back(callback); }

        void Fire(const T&... args) { for (const auto& callback : callbacks) { callback(args); } }
};