#include "ChunksGeneratorThread.h"

ChunksGeneratorThread::ChunksGeneratorThread(const CreateChunk& vCreateChunk) : createChunk{vCreateChunk} {}

void ChunksGeneratorThread::Start() {
    std::array<Chunk::Block, Chunk::Blocks> blocks;
    for (uint x = 0; x < Chunk::Bounds; x++) {
        for (uint z = 0; z < Chunk::Bounds; z++) {
            blocks.at(Chunk::PositionToIndex(Vector3u(x, 0, z))).type = Chunk::Block::Type::Stone;
        }
    }
    createChunk(Vector3u(0, 0, 0), blocks);
}

void ChunksGeneratorThread::Update() {
    // std::cout << "generator update\n";
}

void ChunksGeneratorThread::UpdateCamera(const Vector3f& position) {
    std::lock_guard lock(cameraPositionMutex);
    cameraPosition = position;
}