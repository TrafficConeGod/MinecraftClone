#include "ChunksGeneratorThread.h"
#include <random>

ChunksGeneratorThread::ChunksGeneratorThread(const HasChunk& vHasChunk, const CreateChunk& vCreateChunk, const RemoveChunk& vRemoveChunk, Seed vSeed) : hasChunk{vHasChunk}, createChunk{vCreateChunk}, removeChunk{vRemoveChunk}, seed{vSeed} {}

void ChunksGeneratorThread::Start() {
    
}

void ChunksGeneratorThread::Update(float delta) {
    Vector3i currentGeneratorPosition;
    {
        std::lock_guard lock(cameraPositionMutex);
        currentGeneratorPosition = cameraPosition;
    }
    currentGeneratorPosition /= Chunk::Bounds;
    for (int x = (currentGeneratorPosition.x - chunkGenerationRadius); x < (currentGeneratorPosition.x + chunkGenerationRadius); x++) {
        for (int z = (currentGeneratorPosition.z - chunkGenerationRadius); z < (currentGeneratorPosition.z + chunkGenerationRadius); z++) {
            Vector3i generatingPosition(x, currentGeneratorPosition.y, z);
            if (!hasChunk(generatingPosition)) {
                GenerateChunk(generatingPosition);
            }
        }
    }
}

void ChunksGeneratorThread::GenerateChunk(const Vector3i& position) {
    std::srand(std::time(NULL));
    std::array<Block, Chunk::Blocks> blocks;
    for (uint x = 0; x < Chunk::Bounds; x++) {
        for (uint z = 0; z < Chunk::Bounds; z++) {
            if (std::rand() % 2) {
                blocks.at(Chunk::PositionToIndex(Vector3u(x, 1, z))).type = Block::Type::Grass;
            }
            blocks.at(Chunk::PositionToIndex(Vector3u(x, 0, z))).type = Block::Type::Stone;
        }
    }
    createChunk(position, blocks);
}

void ChunksGeneratorThread::UpdateCamera(const Vector3f& position) {
    std::lock_guard lock(cameraPositionMutex);
    cameraPosition = position;
}