#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "Chunk.h"
#include "Event.h"
#include "ChunksGeneratorThread.h"

class ChunksThread : public virtual Thread {
    public:
        using GraphicsNodeRequested = Event<char>;
    private:
        GraphicsNodeRequested graphicsNodeRequested;
        std::mutex requestedGraphicsNodeMutex;
        EntityReference<GraphicsNode> requestedGraphicsNode = nullptr;

        std::mutex chunksMutex;
        std::vector<EntityReference<Chunk>> chunks;

        EntityReference<ChunksGeneratorThread> chunksGeneratorThread;
    public:
        GIVE_TYPE_ID_1(3, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksThread)
        explicit ChunksThread(const GraphicsNodeRequested& graphicsNodeRequested = GraphicsNodeRequested());
        virtual ~ChunksThread() {}

        virtual void Update() override;

        void CameraUpdate(const Vector3f& position);

        void CreateChunk(const std::array<Chunk::Block, Chunk::BlocksSize>& blocks);

        void RequestedGraphicsNode(EntityReference<GraphicsNode> graphicsNodeRequested);
};