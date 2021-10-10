#include "ChunksThread.h"
#include <iostream>
#include <map>
#include "BlankBlockHandler.h"
#include "GrassBlockHandler.h"
#include "MonoTexturedCubeBlockHandler.h"

ChunksThread::ChunksThread(const CreateChunkGraphicsNode& vCreateChunkGraphicsNode, ChunksGeneratorThread::Seed seed) : createChunkGraphicsNode{vCreateChunkGraphicsNode},chunksGeneratorThread{new ChunksGeneratorThread(std::bind(&ChunksThread::HasChunk, this, std::placeholders::_1), std::bind(&ChunksThread::CreateChunk, this, std::placeholders::_1, std::placeholders::_2), std::bind(&ChunksThread::RemoveChunk, this, std::placeholders::_1), seed)}, blockHandlers{{
    new BlankBlockHandler(),
    new GrassBlockHandler(),
    new MonoTexturedCubeBlockHandler(1),
}} {}

void ChunksThread::Update(float delta) {
    {
        std::lock_guard lock(queuedChunksMutex);
        for (auto& [x, map] : queuedChunks) {
            for (auto& [y, map2] : map) {
                for (auto [z, queuedChunk] : map2) {
                    auto node = createChunkGraphicsNode();
                    if (!chunks.count(queuedChunk.position.x)) {
                        chunks[queuedChunk.position.x] = {};
                    }
                    if (!chunks.at(queuedChunk.position.x).count(queuedChunk.position.y)) {
                        chunks.at(queuedChunk.position.x)[queuedChunk.position.y] = {};
                    }
                    chunks.at(queuedChunk.position.x).at(queuedChunk.position.y).insert(std::pair<uint, EntityReference<Chunk>>(queuedChunk.position.z, new Chunk(queuedChunk.position, queuedChunk.blocks, blockHandlers, node)));
                }
            }
        }
        queuedChunks.clear();
    }
    {
        std::lock_guard lock(chunksMutex);
        for (auto& [x, map] : chunks) {
            for (auto& [y, map2] : map) {
                for (auto [z, chunk] : map2) {
                    chunk->Update();
                }
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
    std::lock_guard lock2(queuedChunksMutex);
    return (chunks.count(position.x) && chunks.at(position.x).count(position.y) && chunks.at(position.x).at(position.y).count(position.z))
    || (queuedChunks.count(position.x) && queuedChunks.at(position.x).count(position.y) && queuedChunks.at(position.x).at(position.y).count(position.z));
}

void ChunksThread::CreateChunk(const Vector3i& position, const std::array<Block, Chunk::Blocks>& blocks) {
    std::lock_guard lock(queuedChunksMutex);
    if (!queuedChunks.count(position.x)) {
        queuedChunks[position.x] = {};
    }
    if (!queuedChunks.at(position.x).count(position.y)) {
        queuedChunks.at(position.x)[position.y] = {};
    }
    queuedChunks.at(position.x).at(position.y).insert(std::pair<uint, QueuedChunk>(position.z, QueuedChunk{ position, blocks }));
}

void ChunksThread::RemoveChunk(const Vector3i& position) {
    std::lock_guard lock(chunksMutex);
    chunks.at(position.x).at(position.y).erase(position.z);
}