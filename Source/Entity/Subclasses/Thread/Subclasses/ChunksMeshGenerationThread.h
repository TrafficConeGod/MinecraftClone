#pragma once
#include "Thread.h"
#include <functional>
#include "Chunk.h"
#include "Vector3.h"
#include "Mono.h"
#include "ChunksGeneratorThread.h"

class ChunksMeshGenerationThread : public virtual Thread {
    public:
        using HasChunkAt = ChunksGeneratorThread::HasChunk;
        using ChunkAt = std::function<EntityReference<Chunk>(const Vector3i&)>;
    private:
        HasChunkAt hasChunkAt;
        ChunkAt chunkAt;

        std::atomic<bool> shouldBeginGenerating = false;
        Mono<std::vector<EntityReference<Chunk>>> chunks;
        std::vector<EntityReference<Chunk>> currentChunks;

        void GenerateChunkMeshAtIfNotInBatch(const Vector3i& position);
    public:
        GIVE_TYPE_ID_1(18, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksMeshGenerationThread)
        explicit ChunksMeshGenerationThread(const HasChunkAt& hasChunkAt, const ChunkAt& chunkAt);
        virtual ~ChunksMeshGenerationThread() {}

        virtual void Update(float delta) override;

        void AddChunk(EntityReference<Chunk> chunk);
        void Generate();
};