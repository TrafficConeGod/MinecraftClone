#include "ChunksGeneratorThread.h"

ChunksGeneratorThread::ChunksGeneratorThread(const HasChunk& vHasChunk, const CreateChunk& vCreateChunk, const RemoveChunk& vRemoveChunk) : hasChunk{vHasChunk}, createChunk{vCreateChunk}, removeChunk{vRemoveChunk} {}

void ChunksGeneratorThread::Start() {
    
}

void ChunksGeneratorThread::Update() {
    Vector3i currentGeneratorPosition;
    {
        std::lock_guard lock(cameraPositionMutex);
        currentGeneratorPosition = cameraPosition;
    }
    currentGeneratorPosition /= Chunk::Bounds;
    for (int x = (currentGeneratorPosition.x - chunkGenerationRadius); x < (currentGeneratorPosition.x + chunkGenerationRadius); x++) {
        for (int y = (currentGeneratorPosition.y - chunkGenerationRadius); y < (currentGeneratorPosition.y + chunkGenerationRadius); y++) {
            for (int z = (currentGeneratorPosition.z - chunkGenerationRadius); z < (currentGeneratorPosition.z + chunkGenerationRadius); z++) {
                Vector3i generatingPosition(x, y, z);
                if (!hasChunk(generatingPosition)) {
                    GenerateChunk(generatingPosition);
                }
            }
        }
    }
}

void ChunksGeneratorThread::GenerateChunk(const Vector3i& position) {
    std::array<Chunk::Block, Chunk::Blocks> blocks;
    for (uint x = 0; x < Chunk::Bounds; x++) {
        for (uint z = 0; z < Chunk::Bounds; z++) {
            blocks.at(Chunk::PositionToIndex(Vector3u(x, 0, z))).type = Chunk::Block::Type::Stone;
        }
    }
    createChunk(position, blocks);
}

void ChunksGeneratorThread::UpdateCamera(const Vector3f& position) {
    std::lock_guard lock(cameraPositionMutex);
    cameraPosition = position;
}