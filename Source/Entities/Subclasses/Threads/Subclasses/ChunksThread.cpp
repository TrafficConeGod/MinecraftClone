#include "ChunksThread.h"
#include <iostream>
#include <map>
#include "BlankBlockHandler.h"
#include "GrassBlockHandler.h"
#include "MonoTexturedCubeBlockHandler.h"

ChunksThread::ChunksThread(const CreateChunkGraphicsNode& vCreateChunkGraphicsNode, Chunk::Seed seed) : createChunkGraphicsNode{vCreateChunkGraphicsNode}, isBlockAtWorldPositionTransparentBind{std::bind(&ChunksThread::IsBlockAtWorldPositionTransparent, this, std::placeholders::_1, std::placeholders::_2)}, chunksGeneratorThread{new ChunksGeneratorThread(std::bind(&ChunksThread::HasChunkAt, this, std::placeholders::_1), std::bind(&ChunksThread::CreateChunk, this, std::placeholders::_1, std::placeholders::_2), std::bind(&ChunksThread::RemoveChunk, this, std::placeholders::_1), std::bind(&ChunksThread::GenerateChunkMeshes, this), seed)}, blockHandlers{{
    new BlankBlockHandler(),
    new GrassBlockHandler(),
    new MonoTexturedCubeBlockHandler(1),
    new MonoTexturedCubeBlockHandler(2)
}} {}

void ChunksThread::Update(float delta) {
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
    if ((chunkMeshGenerationBatch.size() == 0) && shouldBeginChunkMeshGenerationBatch) {
        shouldBeginChunkMeshGenerationBatch = false;
        {
            std::lock_guard lock(chunkMeshGenerationBatchQueueMutex);
            chunkMeshGenerationBatch = chunkMeshGenerationBatchQueue;
            chunkMeshGenerationBatchQueue.clear();
        }
        chunksToGenerateMeshesFor = chunkMeshGenerationBatch;
    }
    if (chunksToGenerateMeshesFor.size() > 0) {
        auto chunk = chunksToGenerateMeshesFor.at(0);
        chunk->UpdateMesh();

        chunksToGenerateMeshesFor.erase(chunksToGenerateMeshesFor.begin());
        if (chunksToGenerateMeshesFor.size() == 0) {
            chunkMeshGenerationBatch.clear();
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
    EntityReference<Chunk> chunk = new Chunk(isBlockAtWorldPositionTransparentBind, blockHandlers, node, position);
    chunk->GenerateBlocks(seed);
    chunks.at(position.x).at(position.y).insert(std::pair<uint, EntityReference<Chunk>>(position.z, chunk));
    std::lock_guard lock2(chunkMeshGenerationBatchQueueMutex);
    chunkMeshGenerationBatchQueue.push_back(chunk);
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
    std::lock_guard lock(chunkMeshGenerationBatchQueueMutex);
    chunkMeshGenerationBatchQueue.push_back(chunk);
}

bool ChunksThread::IsBlockAtWorldPositionTransparent(const Vector3i& worldPosition, const Block& neighborBlock) const {
    if (HasBlockAt(worldPosition)) {
        const auto chunk = ChunkAt(Chunk::WorldPositionToChunkPosition(worldPosition));
        auto localPosition = Chunk::WorldPositionToLocalChunkPosition(worldPosition);
        return chunk->IsBlockAtLocalPositionTransparent(localPosition, neighborBlock);
    }
    return true;
}

void ChunksThread::GenerateChunkMeshes() {
    shouldBeginChunkMeshGenerationBatch = true;
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