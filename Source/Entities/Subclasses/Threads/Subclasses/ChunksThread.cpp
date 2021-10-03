#include "ChunksThread.h"
#include <iostream>

ChunksThread::ChunksThread(const GraphicsNodeRequested& vGraphicsNodeRequested) : graphicsNodeRequested{vGraphicsNodeRequested}, chunksGeneratorThread{new ChunksGeneratorThread(ChunksGeneratorThread::Generated([&](const std::array<Chunk::Block, Chunk::BlocksSize>& blocks) {
    CreateChunk(blocks);
}))} {}

void ChunksThread::Update() {
    std::lock_guard lock(chunksMutex);
    for (auto& chunk : chunks) {
        chunk->Update();
    }
}

void ChunksThread::JoinSubThreads() {
    chunksGeneratorThread->Join();
}

void ChunksThread::CameraUpdate(const Vector3f& position) {
    chunksGeneratorThread->CameraUpdate(position);
}

void ChunksThread::CreateChunk(const std::array<Chunk::Block, Chunk::BlocksSize>& blocks) {
    graphicsNodeRequested.Fire(0);
    std::lock_guard lock(chunksMutex);
    std::lock_guard lock2(requestedGraphicsNodeMutex);
    chunks.push_back(new Chunk(blocks, requestedGraphicsNode));
}

void ChunksThread::RequestedGraphicsNode(EntityReference<GraphicsNode> requestedGraphicsNode) {
    std::lock_guard lock(requestedGraphicsNodeMutex);
    this->requestedGraphicsNode = requestedGraphicsNode;
}