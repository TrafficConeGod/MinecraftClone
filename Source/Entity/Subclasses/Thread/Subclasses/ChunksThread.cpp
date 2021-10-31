#include "ChunksThread.h"
#include <iostream>
#include <map>
#include "BlankBlockHandler.h"
#include "GrassBlockHandler.h"
#include "MonoTexturedCubeBlockHandler.h"
#include "WaterBlockHandler.h"

ChunksThread::ChunksThread(const CreateChunkGraphicsNode& vCreateChunkGraphicsNode, const GraphicsNode::Remove& vRemoveGraphicsNode, Chunk::Seed seed) :
    createChunkGraphicsNode{vCreateChunkGraphicsNode},
    removeGraphicsNode{vRemoveGraphicsNode},
    isBlockAtWorldPositionTransparentBind{std::bind(&ChunksThread::IsBlockAtWorldPositionTransparent, this, std::placeholders::_1, std::placeholders::_2)},
    chunksMeshGenerationThread{new ChunksMeshGenerationThread(
        std::bind(&ChunksThread::HasChunkAt, this, std::placeholders::_1),
        [&](const Vector3i& position) {
            return ChunkAt(position);
        }
    )},
    chunksGeneratorThread{new ChunksGeneratorThread(
        std::bind(&ChunksThread::HasChunkAt, this, std::placeholders::_1),
        std::bind(&ChunksThread::CreateChunk, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&ChunksThread::RemoveChunk, this, std::placeholders::_1),
        std::bind(&ChunksThread::ChunksMeshGenerationThreadGenerate, this),
        seed
    )},
    blockHandlers{{
        new BlankBlockHandler(),
        new GrassBlockHandler(),
        new MonoTexturedCubeBlockHandler(Vector2u(1, 0)),
        new MonoTexturedCubeBlockHandler(Vector2u(2, 0)),
        new WaterBlockHandler(),
        new MonoTexturedCubeBlockHandler(Vector2u(2, 1)),
    }} {}

void ChunksThread::Update(float delta) {
    chunks.Use([&](auto& chunks) {
        for (auto& [x, map] : chunks) {
            for (auto& [y, map2] : map) {
                for (auto [z, chunk] : map2) {
                    chunk->Update();
                }
            }
        }
    });
    {
        if (mouseClicked) {
            mouseClicked = false;
            auto currentMouseClickOrigin = mouseClickOrigin.Value();
            auto currentMouseClickDirection = mouseClickDirection.Value();
            Raycast(currentMouseClickOrigin, currentMouseClickDirection, [&](auto& position) {
                return BlockAt(position).type == Block::Type::Air;
            }, [&](auto& position) {
                return !HasBlockAt(position) || (position - Chunk::FreePositionToGridPosition(currentMouseClickOrigin)).Magnitude() > 10;
            }, [&](auto& position) {
                if (HasBlockAt(position)) {
                    BlockAt(position, {Block::Type::Air});
                }
            });
        }
    }
}

void ChunksThread::JoinSubThreads() {
    chunksMeshGenerationThread->Join();
    chunksGeneratorThread->Join();
}

void ChunksThread::RequestSubThreadsStop() {
    chunksMeshGenerationThread->RequestStop();
    chunksGeneratorThread->RequestStop();
}

void ChunksThread::UpdateCamera(const Vector3f& position) {
    chunksGeneratorThread->UpdateCamera(position);
}

bool ChunksThread::HasChunkAt(const Vector3i& position) const {
    return chunks.UseConstForGet<bool>([position](auto& chunks) {
        return (chunks.count(position.x) && chunks.at(position.x).count(position.y) && chunks.at(position.x).at(position.y).count(position.z));
    }); 
}

EntityReference<Chunk> ChunksThread::ChunkAt(const Vector3i& position) {
    return chunks.UseForGet<EntityReference<Chunk>>([position](auto& chunks) {
        return chunks.at(position.x).at(position.y).at(position.z);
    });
}

const EntityReference<Chunk> ChunksThread::ChunkAt(const Vector3i& position) const {
    return chunks.UseConstForGet<const EntityReference<Chunk>>([position](auto& chunks) {
        return chunks.at(position.x).at(position.y).at(position.z);
    }); 
}

void ChunksThread::CreateChunk(const Vector3i& position, Chunk::Seed seed) {
    chunks.Use([&](auto& chunks) {
        if (!chunks.count(position.x)) {
            chunks[position.x] = {};
        }
        if (!chunks.at(position.x).count(position.y)) {
            chunks.at(position.x)[position.y] = {};
        }
        auto node = createChunkGraphicsNode();
        EntityReference<Chunk> chunk = new Chunk(removeGraphicsNode, isBlockAtWorldPositionTransparentBind, blockHandlers, node, position);
        chunk->GenerateBlocks(seed);
        chunks.at(position.x).at(position.y).insert(std::pair<uint, EntityReference<Chunk>>(position.z, chunk));
        chunksMeshGenerationThread->AddChunk(chunk);
    });
}

void ChunksThread::RemoveChunk(const Vector3i& position) {
    chunks.Use([position](auto& chunks) {
        chunks.at(position.x).at(position.y).erase(position.z);
    });
}

void ChunksThread::SignalMouseClick(const Vector3f& origin, const Vector3f& direction) {
    mouseClicked = true;
    mouseClickOrigin.Value(origin);
    mouseClickDirection.Value(direction);
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
    chunksMeshGenerationThread->AddChunk(chunk);
    chunksMeshGenerationThread->Generate();
}

bool ChunksThread::IsBlockAtWorldPositionTransparent(const Vector3i& worldPosition, const Block& neighborBlock) const {
    if (HasBlockAt(worldPosition)) {
        const auto chunk = ChunkAt(Chunk::WorldPositionToChunkPosition(worldPosition));
        auto localPosition = Chunk::WorldPositionToLocalChunkPosition(worldPosition);
        return chunk->IsBlockAtLocalPositionTransparent(localPosition, neighborBlock);
    }
    return false;
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

void ChunksThread::ChunksMeshGenerationThreadGenerate() {
    chunksMeshGenerationThread->Generate();
}