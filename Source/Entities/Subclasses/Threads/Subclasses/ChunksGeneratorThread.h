#pragma once
#include "Event.h"
#include "Thread.h"
#include <mutex>
#include "Vector3.h"
#include "Chunk.h"

class ChunksGeneratorThread : public virtual Thread {
    public:
        using Generated = Event<const Vector3u&, const std::array<Chunk::Block, Chunk::Blocks>&>;
    private:
        std::mutex cameraPositionMutex;
        Vector3f cameraPosition;

        Vector3f CameraPosition();

        Generated generated;
    protected:
        virtual void Start() override;
        virtual void Update() override;
    public:
        GIVE_TYPE_ID_1(8, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunksGeneratorThread)
        explicit ChunksGeneratorThread(const Generated& generated = Generated());
        virtual ~ChunksGeneratorThread() {}

        void CameraUpdate(const Vector3f& position);
};