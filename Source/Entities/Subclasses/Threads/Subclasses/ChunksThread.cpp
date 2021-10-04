#include "ChunksThread.h"
#include <iostream>

ChunksThread::ChunksThread(const CreateGraphicsNode& vCreateGraphicsNode) : createGraphicsNode{vCreateGraphicsNode}, chunksGeneratorThread{new ChunksGeneratorThread(std::bind(&ChunksThread::CreateChunk, this, std::placeholders::_1, std::placeholders::_2))} {}

void ChunksThread::Update() {
    std::lock_guard lock(chunksMutex);
    for (auto& chunk : chunks) {
        chunk->Update();
    }
}

void ChunksThread::JoinSubThreads() {
    chunksGeneratorThread->Join();
}

void ChunksThread::UpdateCamera(const Vector3f& position) {
    chunksGeneratorThread->UpdateCamera(position);
}

void ChunksThread::CreateChunk(const Vector3u& position, const std::array<Chunk::Block, Chunk::Blocks>& blocks) {
    auto node = createGraphicsNode();
    std::lock_guard lock(chunksMutex);
    chunks.push_back(new Chunk(position, blocks, node));
}