#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "Chunk.h"
#include <functional>
#include "ChunksGeneratorThread.h"
#include <map>
#include <array>
#include "BlockHandler.h"

class ChunksThread : public virtual Thread {
    public:
        using CreateChunkGraphicsNode = std::function<EntityReference<ChunkGraphicsNode>()>;
    private:
        CreateChunkGraphicsNode createChunkGraphicsNode;

        std::mutex chunksMutex;
        std::map<int, std::map<int, std::map<int, EntityReference<Chunk>>>> chunks;

        struct QueuedChunk {
            Vector3i position;
            std::array<Block, Chunk::Blocks> blocks;
        };
        std::mutex queuedChunksMutex;
        std::map<int, std::map<int, std::map<int, QueuedChunk>>> queuedChunks;

        EntityReference<ChunksGeneratorThread> chunksGeneratorThread;

        std::array<EntityReference<BlockHandler>, Block::Types> blockHandlers;

        bool HasChunk(const Vector3i& position);
        void CreateChunk(const Vector3i& position, const std::array<Block, Chunk::Blocks>& blocks);
        void RemoveChunk(const Vector3i& position);
    protected:
        virtual void Update(float delta) override;
        virtual void JoinSubThreads() override;
        virtual void RequestSubThreadsStop() override;
    public:
        GIVE_TYPE_ID_1(3, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksThread)
        explicit ChunksThread(const CreateChunkGraphicsNode& createChunkGraphicsNode, ChunksGeneratorThread::Seed seed);
        virtual ~ChunksThread() {}

        void UpdateCamera(const Vector3f& position);
};