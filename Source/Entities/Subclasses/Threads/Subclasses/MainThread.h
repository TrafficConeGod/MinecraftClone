#pragma once
#include "Thread.h"

class MainThread : public virtual Thread {
    public:
        virtual ~MainThread() {}

        virtual void Update();
};