#pragma once
#include "Entity.h"
#include <thread>
#include <functional>

class Thread : public virtual Entity {
    private:
        std::thread worker;
    public:
        GIVE_TYPE_ID_1(1, Entity)

        DELETE_ILLEGAL_CONSTRUCTORS(Thread)
        explicit Thread();
        virtual ~Thread() {}

        virtual void Update() {}

        void Join();
    public:
        using MutexId = std::size_t;
        
        static constexpr MutexId ConsoleOutput = 0;

        static void Capture(MutexId mutex, std::function<void()> context);
};