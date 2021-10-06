#include "ChunksThread.h"
#include <iostream>
#include <map>

ChunksThread::ChunksThread(const CreateChunkGraphicsNode& vCreateChunkGraphicsNode) : createChunkGraphicsNode{vCreateChunkGraphicsNode}, chunksGeneratorThread{new ChunksGeneratorThread(std::bind(&ChunksThread::HasChunk, this, std::placeholders::_1), std::bind(&ChunksThread::CreateChunk, this, std::placeholders::_1, std::placeholders::_2), std::bind(&ChunksThread::RemoveChunk, this, std::placeholders::_1))} {}

void ChunksThread::Update(float delta) {
    std::lock_guard lock(chunksMutex);
    for (auto& [x, map] : chunks) {
        for (auto& [y, map2] : map) {
            for (auto [z, chunk] : map2) {
                chunk->Update();
            }
        }
    }
}

void ChunksThread::JoinSubThreads() {
    chunksGeneratorThread->Join();
}

void ChunksThread::RequestSubThreadsStop() {
    chunksGeneratorThread->RequestStop();
}

void ChunksThread::UpdateCamera(const Vector3f& position) {
    chunksGeneratorThread->UpdateCamera(position);
}

bool ChunksThread::HasChunk(const Vector3i& position) {
    std::lock_guard lock(chunksMutex);
    return chunks.count(position.x) && chunks.at(position.x).count(position.y) && chunks.at(position.x).at(position.y).count(position.z);
}

void ChunksThread::CreateChunk(const Vector3i& position, const std::array<Chunk::Block, Chunk::Blocks>& blocks) {
    auto node = createChunkGraphicsNode();
    std::lock_guard lock(chunksMutex);
    if (!chunks.count(position.x)) {
        chunks[position.x] = {};
    }
    if (!chunks.at(position.x).count(position.y)) {
        chunks.at(position.x)[position.y] = {};
    }
    chunks.at(position.x).at(position.y).insert(std::pair<uint, EntityReference<Chunk>>(position.z, new Chunk(position, blocks, node)));
}

void ChunksThread::RemoveChunk(const Vector3i& position) {
    std::lock_guard lock(chunksMutex);
    chunks.at(position.x).at(position.y).erase(position.z);
}