#include "ChunksGeneratorThread.h"
#include <random>

ChunksGeneratorThread::ChunksGeneratorThread(const HasChunk& vHasChunk, const CreateChunk& vCreateChunk, const RemoveChunk& vRemoveChunk, const GenerateChunkMeshes& vGenerateChunkMeshes, Chunk::Seed vSeed) : hasChunk{vHasChunk}, createChunk{vCreateChunk}, removeChunk{vRemoveChunk}, generateChunkMeshes{vGenerateChunkMeshes}, seed{vSeed} {}

void ChunksGeneratorThread::Update(float delta) {
    Vector3i currentGeneratorPosition = cameraPosition.Value();
    currentGeneratorPosition /= Chunk::Bounds;

    bool generated = false;
    for (int x = (currentGeneratorPosition.x - chunkGenerationRadius); x < (currentGeneratorPosition.x + chunkGenerationRadius); x++) {
        for (int y = (currentGeneratorPosition.y - chunkGenerationRadius); y < (currentGeneratorPosition.y + chunkGenerationRadius); y++) {
            for (int z = (currentGeneratorPosition.z - chunkGenerationRadius); z < (currentGeneratorPosition.z + chunkGenerationRadius); z++) {
                Vector3i position = Vector3i(x, y, z);
                if (!hasChunk(position)) {
                    GenerateChunk(position);
                    generated = true;
                }
            }
        }
    }
    std::size_t index = 0;
    for (const auto& position : generatedChunkPositions) {
        if ((position - currentGeneratorPosition).SquareMagnitude() > chunkGenerationRadius) {
            removeChunk(position);
            generatedChunkPositions.erase(generatedChunkPositions.begin() + index);
            break;
        }
        index++;
    }
    if (generated) {
        generateChunkMeshes();
    }
}

void ChunksGeneratorThread::GenerateChunk(const Vector3i& position) {
    generatedChunkPositions.push_back(position);
    createChunk(position, seed);
}

void ChunksGeneratorThread::UpdateCamera(const Vector3f& position) {
    cameraPosition.Value(position);
}