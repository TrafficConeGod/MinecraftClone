#pragma once
#include <mutex>
#include <functional>

template<typename S>
class Mono {
    private:
        mutable std::mutex mutex;
        S value;
    public:
        Mono() {}
        Mono(const S& vValue) : value{vValue} {}
        
        Mono(const Mono&) = delete;
        Mono(Mono&&) = delete;
        void operator=(const Mono&) = delete;

        void Use(std::function<void(S&)> context) { std::lock_guard lock(mutex); context(value); }
        void UseConst(std::function<void(const S&)> context) const { std::lock_guard lock(mutex); context(value); }

        template<typename T>
        T UseForGet(std::function<T(S&)> context) { std::lock_guard lock(mutex); return context(value); }
        template<typename T>
        T UseConstForGet(std::function<T(const S&)> context) const { std::lock_guard lock(mutex); return context(value); }
        
        S Value() const { std::lock_guard lock(mutex); return value; }
        void Value(const S& value) { std::lock_guard lock(mutex); this->value = value; }
};