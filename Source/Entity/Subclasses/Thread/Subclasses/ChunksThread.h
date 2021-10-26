#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "Chunk.h"
#include <functional>
#include "ChunksGeneratorThread.h"
#include "ChunksMeshGenerationThread.h"
#include <map>
#include <array>
#include "SingleUsage.h"
#include "BlockHandler.h"

class ChunksThread : public virtual Thread {
    public:
        using CreateChunkGraphicsNode = std::function<EntityReference<ChunkGraphicsNode>()>;
    private:
        CreateChunkGraphicsNode createChunkGraphicsNode;
        Chunk::IsBlockAtWorldPositionTransparent isBlockAtWorldPositionTransparentBind;

        SingleUsage<std::map<int, std::map<int, std::map<int, EntityReference<Chunk>>>>> chunks;

        EntityReference<ChunksMeshGenerationThread> chunksMeshGenerationThread;
        EntityReference<ChunksGeneratorThread> chunksGeneratorThread;

        std::array<EntityReference<BlockHandler>, Block::Types> blockHandlers;

        std::atomic<bool> mouseClicked = false;
        SingleUsage<Vector3f> mouseClickOrigin;
        SingleUsage<Vector3f> mouseClickDirection;

        bool HasChunkAt(const Vector3i& position) const;
        EntityReference<Chunk> ChunkAt(const Vector3i& position);
        const EntityReference<Chunk> ChunkAt(const Vector3i& position) const;
        const EntityReference<Chunk> ChunkAtConst(const Vector3i& position) const;
        bool HasQueuedOrActualChunkAt(const Vector3i& position) const;
        void CreateChunk(const Vector3i& position, Chunk::Seed seed);
        void RemoveChunk(const Vector3i& position);
        
        void Raycast(const Vector3f& origin, const Vector3f& direction, const std::function<bool(const Vector3i&)>& canContinue, const std::function<bool(const Vector3i&)>& shouldStop, const std::function<void(const Vector3i&)>& hitCallback) const;
        bool HasBlockAt(const Vector3i& position) const;
        const Block& BlockAt(const Vector3i& position) const;
        void BlockAt(const Vector3i& position, const Block& block);
        bool IsBlockAtWorldPositionTransparent(const Vector3i& position, const Block& neighborBlock) const;

        void ChunksMeshGenerationThreadGenerate();
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