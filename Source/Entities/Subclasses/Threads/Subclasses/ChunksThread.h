#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "Chunk.h"
#include <functional>
#include "ChunksGeneratorThread.h"
#include <map>

class ChunksThread : public virtual Thread {
    public:
        using CreateChunkGraphicsNode = std::function<EntityReference<ChunkGraphicsNode>()>;
    private:
        CreateChunkGraphicsNode createChunkGraphicsNode;

        std::mutex chunksMutex;
        std::map<int, std::map<int, std::map<int, EntityReference<Chunk>>>> chunks;

        EntityReference<ChunksGeneratorThread> chunksGeneratorThread;

        bool HasChunk(const Vector3i& position);
        void CreateChunk(const Vector3i& position, const std::array<Chunk::Block, Chunk::Blocks>& blocks);
        void RemoveChunk(const Vector3i& position);
    protected:
        virtual void Update(float delta) override;
        virtual void JoinSubThreads() override;
        virtual void RequestSubThreadsStop() override;
    public:
        GIVE_TYPE_ID_1(3, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksThread)
        explicit ChunksThread(const CreateChunkGraphicsNode& createChunkGraphicsNode);
        virtual ~ChunksThread() {}

        void UpdateCamera(const Vector3f& position);
};