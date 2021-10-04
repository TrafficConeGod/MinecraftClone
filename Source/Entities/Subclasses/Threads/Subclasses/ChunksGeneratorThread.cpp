#include "ChunksGeneratorThread.h"

ChunksGeneratorThread::ChunksGeneratorThread(const CreateChunk& vCreateChunk) : createChunk{vCreateChunk} {}

void ChunksGeneratorThread::Start() {
    std::array<Chunk::Block, Chunk::Blocks> blocks;
    blocks.at(99).type = Chunk::Block::Type::Stone;
    blocks.at(100).type = Chunk::Block::Type::Stone;
    createChunk(Vector3u(0, 0, 0), blocks);
}

void ChunksGeneratorThread::Update() {
    // std::cout << "generator update\n";
}

void ChunksGeneratorThread::UpdateCamera(const Vector3f& position) {
    std::lock_guard<std::mutex> lock(cameraPositionMutex);
    cameraPosition = position;
}