#pragma once
#include "Event.h"
#include "Thread.h"
#include <mutex>
#include "Vector3.h"
#include "Chunk.h"

class ChunksGeneratorThread : public virtual Thread {
    public:
        using Generated = Event<const std::array<Chunk::Block, Chunk::BlocksSize>&>;
    private:
        std::mutex cameraPositionMutex;
        Vector3f cameraPosition;

        Vector3f CameraPosition();

        Generated generated;
    public:
        GIVE_TYPE_ID_1(8, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksGeneratorThread)
        explicit ChunksGeneratorThread(const Generated& generated = Generated());
        virtual ~ChunksGeneratorThread() {}

        virtual void Update() override;

        void CameraUpdate(const Vector3f& position);
};