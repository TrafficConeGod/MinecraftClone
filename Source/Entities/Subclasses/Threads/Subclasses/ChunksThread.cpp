#include "ChunksThread.h"
#include <iostream>

ChunksThread::ChunksThread(const GraphicsNodeRequested& vGraphicsNodeRequested) : graphicsNodeRequested{vGraphicsNodeRequested}, chunksGeneratorThread{new ChunksGeneratorThread(ChunksGeneratorThread::Generated(std::bind(&ChunksThread::CreateChunk, this, std::placeholders::_1, std::placeholders::_2)))} {}

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

void ChunksThread::CreateChunk(const Vector3u& position, const std::array<Chunk::Block, Chunk::Blocks>& blocks) {
    graphicsNodeRequested.Fire(0);
    std::lock_guard lock(chunksMutex);
    std::lock_guard lock2(requestedGraphicsNodeMutex);
    chunks.push_back(new Chunk(position, blocks, requestedGraphicsNode));
}

void ChunksThread::RequestedGraphicsNode(EntityReference<GraphicsNode> requestedGraphicsNode) {
    std::lock_guard lock(requestedGraphicsNodeMutex);
    this->requestedGraphicsNode = requestedGraphicsNode;
}