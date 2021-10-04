#pragma once
#include <functional>
#include "Thread.h"
#include <mutex>
#include "Vector3.h"
#include "Chunk.h"

class ChunksGeneratorThread : public virtual Thread {
    public:
        using CreateChunk = std::function<void(const Vector3u&, const std::array<Chunk::Block, Chunk::Blocks>&)>;
    private:
        std::mutex cameraPositionMutex;
        Vector3f cameraPosition;

        Vector3f CameraPosition();

        CreateChunk createChunk;
    protected:
        virtual void Start() override;
        virtual void Update() override;
    public:
        GIVE_TYPE_ID_1(8, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksGeneratorThread)
        explicit ChunksGeneratorThread(const CreateChunk& createChunk);
        virtual ~ChunksGeneratorThread() {}

        void UpdateCamera(const Vector3f& position);
};