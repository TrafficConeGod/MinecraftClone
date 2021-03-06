#pragma once
#include <functional>
#include "Thread.h"
#include "Mono.h"
#include "Vector3.h"
#include "Chunk.h"

class ChunksGeneratorThread : public virtual Thread {
    public:
        using HasChunk = std::function<bool(const Vector3i&)>;
        using CreateChunk = std::function<void(const Vector3i&, Chunk::Seed)>;
        using RemoveChunk = std::function<void(const Vector3i&)>;
        using GenerateChunkMeshes = std::function<void()>;
    private:
        HasChunk hasChunk;
        CreateChunk createChunk;
        RemoveChunk removeChunk;
        GenerateChunkMeshes generateChunkMeshes;


        Mono<Vector3f> cameraPosition;

        Chunk::Seed seed;

        int chunkGenerationRadius = 5;

        std::vector<Vector3i> generatedChunkPositions;

        void GenerateChunk(const Vector3i& position);
    protected:
        virtual void Update(float delta) override;
    public:
        GIVE_TYPE_ID_1(8, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksGeneratorThread)
        explicit ChunksGeneratorThread(const HasChunk& hasChunk, const CreateChunk& createChunk, const RemoveChunk& removeChunk, const GenerateChunkMeshes& generateChunkMeshes, Chunk::Seed seed);
        virtual ~ChunksGeneratorThread() {}

        void UpdateCamera(const Vector3f& position);
};