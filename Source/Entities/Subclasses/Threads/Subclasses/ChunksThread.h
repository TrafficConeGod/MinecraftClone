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
        Chunk::IsBlockAtWorldPositionTransparent isBlockAtWorldPositionTransparentBind;

        mutable std::mutex chunksMutex;
        std::map<int, std::map<int, std::map<int, EntityReference<Chunk>>>> chunks;

        mutable std::mutex chunkMeshGenerationBatchQueueMutex;
        std::vector<EntityReference<Chunk>> chunkMeshGenerationBatchQueue;

        std::atomic<bool> shouldBeginChunkMeshGenerationBatch = false;
        std::vector<EntityReference<Chunk>> chunkMeshGenerationBatch;
        std::vector<EntityReference<Chunk>> chunksToGenerateMeshesFor;
        std::vector<EntityReference<Chunk>> alreadyMeshGeneratedChunks;

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
        void CreateChunk(const Vector3i& position, Chunk::Seed seed);
        void RemoveChunk(const Vector3i& position);
        
        void Raycast(const Vector3f& origin, const Vector3f& direction, const std::function<bool(const Vector3i&)>& canContinue, const std::function<bool(const Vector3i&)>& shouldStop, const std::function<void(const Vector3i&)>& hitCallback) const;
        bool HasBlockAt(const Vector3i& position) const;
        const Block& BlockAt(const Vector3i& position) const;
        void BlockAt(const Vector3i& position, const Block& block);
        void GenerateChunkMeshes();
        bool IsBlockAtWorldPositionTransparent(const Vector3i& position, const Block& neighborBlock) const;
        void GenerateChunkMeshAtIfNotInBatch(const Vector3i& position);
    protected:
        virtual void Update(float delta) override;
        virtual void JoinSubThreads() override;
        virtual void RequestSubThreadsStop() override;
    public:
        GIVE_TYPE_ID_1(3, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksThread)
        explicit ChunksThread(const CreateChunkGraphicsNode& createChunkGraphicsNode, Chunk::Seed seed);
        virtual ~ChunksThread() {}

        void UpdateCamera(const Vector3f& position);
        void SignalMouseClick(const Vector3f& origin, const Vector3f& direction);
};