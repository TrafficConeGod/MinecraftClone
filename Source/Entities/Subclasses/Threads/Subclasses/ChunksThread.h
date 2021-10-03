#pragma once
#include "Thread.h"

class ChunksThread : public virtual Thread {
    public:
        GIVE_TYPE_ID_1(3, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksThread)
        explicit ChunksThread();
        virtual ~ChunksThread() {}

        virtual void Update();
};