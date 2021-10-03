#include "ChunksGeneratorThread.h"

ChunksGeneratorThread::ChunksGeneratorThread(const Generated& vGenerated) : generated{vGenerated} {}

void ChunksGeneratorThread::Start() {
    std::array<Chunk::Block, Chunk::BlocksSize> blocks;
    blocks.at(100) = { Chunk::Block::Type::Stone };
    generated.Fire(blocks);
}

void ChunksGeneratorThread::Update() {
    std::cout << "generator update\n";
}

void ChunksGeneratorThread::CameraUpdate(const Vector3f& position) {
    std::lock_guard<std::mutex> lock(cameraPositionMutex);
    cameraPosition = position;
}