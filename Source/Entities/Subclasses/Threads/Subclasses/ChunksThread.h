#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "Chunk.h"
#include <functional>
#include "ChunksGeneratorThread.h"
#include <map>
#include <array>
#include <mutex>
#include "BlockHandler.h"

class ChunksThread : public virtual Thread {
    public:
        using CreateChunkGraphicsNode = std::function<EntityReference<ChunkGraphicsNode>()>;
    private:
        CreateChunkGraphicsNode createChunkGraphicsNode;

        mutable std::mutex chunksMutex;
        std::map<int, std::map<int, std::map<int, EntityReference<Chunk>>>> chunks;

        struct QueuedChunk {
            Vector3i position;
            std::array<Block, Chunk::Blocks> blocks;
        };
        mutable std::mutex queuedChunksMutex;
        std::map<int, std::map<int, std::map<int, QueuedChunk>>> queuedChunks;

        EntityReference<ChunksGeneratorThread> chunksGeneratorThread;

        std::array<EntityReference<BlockHandler>, Block::Types> blockHandlers;

        std::mutex mouseClickMutex;
        bool mouseClicked = false;
        Vector3f mouseClickOrigin;
        Vector3f mouseClickDirection;

        bool HasChunkAt(const Vector3i& position) const;
        EntityReference<Chunk> ChunkAt(const Vector3i& position);
        const EntityReference<Chunk> ChunkAt(const Vector3i& position) const;
        bool HasQueuedOrActualChunkAt(const Vector3i& position) const;
        void CreateChunk(const Vector3i& position, const std::array<Block, Chunk::Blocks>& blocks);
        void RemoveChunk(const Vector3i& position);
        
        void Raycast(const Vector3f& origin, const Vector3f& direction, const std::function<bool(const Vector3i&)>& continueCheck, const std::function<void(const Vector3i&)>& callback) const;
        bool HasBlockAt(const Vector3i& position) const;
        const Block& BlockAt(const Vector3i& position) const;
        void BlockAt(const Vector3i& position, const Block& block);
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
        void SignalMouseClick(const Vector3f& origin, const Vector3f& direction);
};