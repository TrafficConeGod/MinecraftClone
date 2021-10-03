#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "GraphicsNode.h"

class RenderThread : public virtual Thread {
    private:
        std::vector<EntityReference<GraphicsNode>> nodes;
    public:
        GIVE_TYPE_ID_1(4, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(RenderThread)
        explicit RenderThread(const std::vector<EntityReference<GraphicsNode>>& nodes);
        virtual ~RenderThread() {}

        virtual void Update() override;
};