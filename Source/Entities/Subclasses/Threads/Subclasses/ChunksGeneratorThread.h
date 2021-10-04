#pragma once
#include <functional>
#include "Thread.h"
#include <mutex>
#include "Vector3.h"
#include "Chunk.h"

class ChunksGeneratorThread : public virtual Thread {
    public:
        using HasChunk = std::function<bool(const Vector3i&)>;
        using CreateChunk = std::function<void(const Vector3i&, const std::array<Chunk::Block, Chunk::Blocks>&)>;
        using RemoveChunk = std::function<void(const Vector3i&)>;
    private:
        std::mutex cameraPositionMutex;
        Vector3f cameraPosition;

        Vector3f CameraPosition();

        HasChunk hasChunk;
        CreateChunk createChunk;
        RemoveChunk removeChunk;

        int chunkGenerationRadius = 5;

        void GenerateChunk(const Vector3i& position);
    protected:
        virtual void Start() override;
        virtual void Update() override;
    public:
        GIVE_TYPE_ID_1(8, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksGeneratorThread)
        explicit ChunksGeneratorThread(const HasChunk& hasChunk, const CreateChunk& createChunk, const RemoveChunk& removeChunk);
        virtual ~ChunksGeneratorThread() {}

        void UpdateCamera(const Vector3f& position);
};