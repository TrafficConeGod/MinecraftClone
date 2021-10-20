#include "ChunksThread.h"
#include <iostream>
#include <map>
#include "BlankBlockHandler.h"
#include "GrassBlockHandler.h"
#include "MonoTexturedCubeBlockHandler.h"

ChunksThread::ChunksThread(const CreateChunkGraphicsNode& vCreateChunkGraphicsNode, ChunksGeneratorThread::Seed seed) : createChunkGraphicsNode{vCreateChunkGraphicsNode},chunksGeneratorThread{new ChunksGeneratorThread(std::bind(&ChunksThread::HasQueuedOrActualChunkAt, this, std::placeholders::_1), std::bind(&ChunksThread::CreateChunk, this, std::placeholders::_1, std::placeholders::_2), std::bind(&ChunksThread::RemoveChunk, this, std::placeholders::_1), seed)}, blockHandlers{{
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
    {
        std::lock_guard lock(mouseClickMutex);
        if (mouseClicked) {
            mouseClicked = false;
            Raycast(mouseClickOrigin, mouseClickDirection, [&](const auto& position) {
                return HasBlockAt(position) && BlockAt(position).type != Block::Type::Air;
            }, [&](const auto& position) {
                BlockAt(position, {Block::Type::Air});
            });
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

bool ChunksThread::HasChunkAt(const Vector3i& position) const {
    std::lock_guard lock(chunksMutex);
    return (chunks.count(position.x) && chunks.at(position.x).count(position.y) && chunks.at(position.x).at(position.y).count(position.z));
}

EntityReference<Chunk> ChunksThread::ChunkAt(const Vector3i& position) {
    return chunks.at(position.x).at(position.y).at(position.z);
}

const EntityReference<Chunk> ChunksThread::ChunkAt(const Vector3i& position) const {
    return chunks.at(position.x).at(position.y).at(position.z);
}

bool ChunksThread::HasQueuedOrActualChunkAt(const Vector3i& position) const {
    std::lock_guard lock(queuedChunksMutex);
    return HasChunkAt(position) || (queuedChunks.count(position.x) && queuedChunks.at(position.x).count(position.y) && queuedChunks.at(position.x).at(position.y).count(position.z));
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

void ChunksThread::SignalMouseClick(const Vector3f& origin, const Vector3f& direction) {
    std::lock_guard lock(mouseClickMutex);
    mouseClicked = true;
    mouseClickOrigin = origin;
    mouseClickDirection = direction;
}

bool ChunksThread::HasBlockAt(const Vector3i& position) const {
    return HasChunkAt(Chunk::WorldPositionToChunkPosition(position));
}

const Block& ChunksThread::BlockAt(const Vector3i& position) const {
    auto chunk = ChunkAt(Chunk::WorldPositionToChunkPosition(position));
    return chunk->BlockAt(Chunk::WorldPositionToLocalChunkPosition(position));
}

void ChunksThread::BlockAt(const Vector3i& position, const Block& block) {
    auto chunk = ChunkAt(Chunk::WorldPositionToChunkPosition(position));
    chunk->BlockAt(Chunk::WorldPositionToLocalChunkPosition(position), block);
}

void ChunksThread::Raycast(const Vector3f& origin, const Vector3f& direction, const std::function<bool(const Vector3i&)>& canContinue, const std::function<void(const Vector3i&)>& hitCallback) const {
    if (direction.Magnitude() == 0) {
        return;
    }
    Vector3f position = origin;
    for (;;) {
        if (canContinue(position)) {
            position += direction;
        } else {
            hitCallback(position);
            return;
        }
    }
}