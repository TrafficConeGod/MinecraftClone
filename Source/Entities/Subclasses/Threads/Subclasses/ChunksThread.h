#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "GraphicsNode.h"

class ChunksThread : public virtual Thread {
    private:
        EntityReference<GraphicsNode> node;
    public:
        GIVE_TYPE_ID_1(3, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksThread)
        explicit ChunksThread(EntityReference<GraphicsNode> node);
        virtual ~ChunksThread() {}

        virtual void Update() override;
};