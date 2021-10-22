#include "ChunksThread.h"
#include <iostream>
#include <map>
#include "BlankBlockHandler.h"
#include "GrassBlockHandler.h"
#include "MonoTexturedCubeBlockHandler.h"

ChunksThread::ChunksThread(const CreateChunkGraphicsNode& vCreateChunkGraphicsNode, Chunk::Seed seed) : createChunkGraphicsNode{vCreateChunkGraphicsNode},chunksGeneratorThread{new ChunksGeneratorThread(std::bind(&ChunksThread::HasChunkAt, this, std::placeholders::_1), std::bind(&ChunksThread::CreateChunk, this, std::placeholders::_1, std::placeholders::_2), std::bind(&ChunksThread::RemoveChunk, this, std::placeholders::_1), std::bind(&ChunksThread::GenerateChunkMeshes, this), seed)}, blockHandlers{{
    new BlankBlockHandler(),
    new GrassBlockHandler(),
    new MonoTexturedCubeBlockHandler(1),
}} {}

void ChunksThread::Update(float delta) {
    {
        bool generatedMesh = false;
        std::lock_guard lock(chunksMutex);
        for (auto& [x, map] : chunks) {
            for (auto& [y, map2] : map) {
                for (auto [z, chunk] : map2) {
                    if (!generatedMesh) {
                        if (chunk->UpdateMeshIfNeedsGeneration()) {
                            generatedMesh = true;
                        }
                    }
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
                return BlockAt(position).type == Block::Type::Air;
            }, [&](const auto& position) {
                return !HasBlockAt(position) || (position - Chunk::FreePositionToGridPosition(mouseClickOrigin)).Magnitude() > 10;
            }, [&](const auto& position) {
                if (HasBlockAt(position)) {
                    BlockAt(position, {Block::Type::Air});
                }
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
    std::lock_guard lock(chunksMutex);
    return chunks.at(position.x).at(position.y).at(position.z);
}

const EntityReference<Chunk> ChunksThread::ChunkAt(const Vector3i& position) const {
    std::lock_guard lock(chunksMutex);
    return chunks.at(position.x).at(position.y).at(position.z);
}

void ChunksThread::CreateChunk(const Vector3i& position, Chunk::Seed seed) {
    std::lock_guard lock(chunksMutex);
    if (!chunks.count(position.x)) {
        chunks[position.x] = {};
    }
    if (!chunks.at(position.x).count(position.y)) {
        chunks.at(position.x)[position.y] = {};
    }
    auto node = createChunkGraphicsNode();
    EntityReference<Chunk> chunk = new Chunk(blockHandlers, node, position);
    chunk->GenerateBlocks(seed);
    chunks.at(position.x).at(position.y).insert(std::pair<uint, EntityReference<Chunk>>(position.z, chunk));
    chunksToGenerateMeshesFor.push_back(chunk);
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

void ChunksThread::GenerateChunkMeshes() {
    std::lock_guard lock(chunksMutex);
    for (auto chunk : chunksToGenerateMeshesFor) {
        chunk->MakeMeshGenerate();
    }
    chunksToGenerateMeshesFor.clear();
}

void ChunksThread::Raycast(const Vector3f& origin, const Vector3f& direction, const std::function<bool(const Vector3i&)>& canContinue, const std::function<bool(const Vector3i&)>& shouldEnd, const std::function<void(const Vector3i&)>& hitCallback) const {
    if (direction.Magnitude() == 0) {
        return;
    }
    Vector3f position = origin;
    for (;;) {
        auto gridPosition = Chunk::FreePositionToGridPosition(position);
        if (shouldEnd(gridPosition)) {
            return;
        }
        if (canContinue(gridPosition)) {
            position += direction;
        } else {
            hitCallback(gridPosition);
            return;
        }
    }
}