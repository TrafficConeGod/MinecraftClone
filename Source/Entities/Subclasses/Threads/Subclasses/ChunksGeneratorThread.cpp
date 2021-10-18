#include "ChunksGeneratorThread.h"
#include <random>
#include "PerlinNoise.hpp"

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
    const siv::PerlinNoise perlin(seed);
    std::array<Block, Chunk::Blocks> blocks;
    for (uint x = 0; x < Chunk::Bounds; x++) {
        for (uint y = 0; y < Chunk::Bounds; y++) {
            for (uint z = 0; z < Chunk::Bounds; z++) {
                Vector3u localPosition(x, y, z);
                auto worldPosition = Chunk::LocalChunkPositionToWorldPosition(position, localPosition);
                auto height = perlin.accumulatedOctaveNoise2D_0_1(worldPosition.x / 16.f, worldPosition.z / 16.f, 8);

                int yPos = height * MaxGenerationHeight;

                if (worldPosition.y == yPos) {
                    blocks.at(Chunk::PositionToIndex(localPosition)).type = Block::Type::Grass;
                } else if (worldPosition.y < yPos) {
                    blocks.at(Chunk::PositionToIndex(localPosition)).type = Block::Type::Stone;
                }
            }
        }
    }
    createChunk(position, blocks);
}

void ChunksGeneratorThread::UpdateCamera(const Vector3f& position) {
    std::lock_guard lock(cameraPositionMutex);
    cameraPosition = position;
}