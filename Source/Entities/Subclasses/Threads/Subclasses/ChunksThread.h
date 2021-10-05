#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "Chunk.h"
#include <functional>
#include "ChunksGeneratorThread.h"
#include <map>

class ChunksThread : public virtual Thread {
    public:
        using CreateGraphicsNode = std::function<EntityReference<GraphicsNode>()>;
    private:
        CreateGraphicsNode createGraphicsNode;

        std::mutex chunksMutex;
        std::map<int, std::map<int, std::map<int, EntityReference<Chunk>>>> chunks;

        EntityReference<ChunksGeneratorThread> chunksGeneratorThread;

        bool HasChunk(const Vector3i& position);
        void CreateChunk(const Vector3i& position, const std::array<Chunk::Block, Chunk::Blocks>& blocks);
        void RemoveChunk(const Vector3i& position);
    protected:
        virtual void Update(float delta) override;
    public:
        GIVE_TYPE_ID_1(3, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksThread)
        explicit ChunksThread(const CreateGraphicsNode& createGraphicsNode);
        virtual ~ChunksThread() {}
        virtual void JoinSubThreads() override;

        void UpdateCamera(const Vector3f& position);
};