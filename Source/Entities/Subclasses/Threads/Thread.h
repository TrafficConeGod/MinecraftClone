#pragma once
#include "Entity.h"
#include <thread>
#include <functional>
#include <atomic>

class Thread : public virtual Entity {
    private:
        bool joined = false;
        std::thread worker;

        std::atomic<bool> active = true;
    protected:
        virtual void Start() {}
        virtual void Update(float delta) {}
        virtual void Stop() {}

        virtual void RequestSubThreadsStop() {}
    public:
        GIVE_TYPE_ID_1(1, Entity)

        DELETE_ILLEGAL_CONSTRUCTORS(Thread)
        explicit Thread();
        virtual ~Thread();

        virtual void JoinSubThreads() {}

        void Join();
        void RequestStop();
};