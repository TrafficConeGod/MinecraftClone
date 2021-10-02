#pragma once
#include "Entity.h"
#include <thread>
#include <functional>

class Thread : public virtual Entity {
    private:
        std::thread worker;
    public:
        static constexpr Entity::TypeId Type = 1;

        static constexpr bool IsOfType(Entity::TypeId type) {
            return type == Type || Entity::IsOfType(type);
        }

        explicit Thread();
        virtual ~Thread() {}

        virtual void Update() {}

        void Join();
    public:
        using MutexId = std::size_t;
        
        static constexpr MutexId ConsoleOutput = 0;

        static void Capture(MutexId mutex, std::function<void()> context);
};