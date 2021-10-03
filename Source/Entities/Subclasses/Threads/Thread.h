#pragma once
#include "Entity.h"
#include <thread>
#include <functional>
#include <atomic>

class Thread : public virtual Entity {
    private:
        std::thread worker;

        std::atomic<bool> active = true;
    public:
        GIVE_TYPE_ID_1(1, Entity)

        DELETE_ILLEGAL_CONSTRUCTORS(Thread)
        explicit Thread();
        virtual ~Thread();

        virtual void Start() {}
        virtual void Update() {}
        virtual void Stop() {}

        void Join();
};