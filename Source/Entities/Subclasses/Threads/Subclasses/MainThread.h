#pragma once
#include "Thread.h"

class MainThread : public virtual Thread {
    private:
        int x = 0;
    public:
        virtual ~MainThread() {}

        virtual void Update();
};