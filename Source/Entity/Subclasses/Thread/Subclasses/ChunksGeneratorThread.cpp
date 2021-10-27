#include "ChunksGeneratorThread.h"
#include <random>

ChunksGeneratorThread::ChunksGeneratorThread(const HasChunk& vHasChunk, const CreateChunk& vCreateChunk, const RemoveChunk& vRemoveChunk, const GenerateChunkMeshes& vGenerateChunkMeshes, Chunk::Seed vSeed) : hasChunk{vHasChunk}, createChunk{vCreateChunk}, removeChunk{vRemoveChunk}, generateChunkMeshes{vGenerateChunkMeshes}, seed{vSeed} {}

void ChunksGeneratorThread::Update(float delta) {
    Vector3i currentGeneratorPosition = cameraPosition.Value();
    currentGeneratorPosition /= Chunk::Bounds;

    bool generated = false;
    for (int x = (currentGeneratorPosition.x - chunkGenerationRadius.x); x < (currentGeneratorPosition.x + chunkGenerationRadius.x); x++) {
        for (int y = (currentGeneratorPosition.y - chunkGenerationRadius.y); y < (currentGeneratorPosition.y + chunkGenerationRadius.y); y++) {
            for (int z = (currentGeneratorPosition.z - chunkGenerationRadius.z); z < (currentGeneratorPosition.z + chunkGenerationRadius.z); z++) {
                Vector3i position = Vector3i(x, y, z);
                if (!hasChunk(position)) {
                    createChunk(position, seed);
                    generated = true;
                }
            }
        }
    }
    if (generated) {
        generateChunkMeshes();
    }
}

void ChunksGeneratorThread::GenerateChunk(const Vector3i& position) {
    createChunk(position, seed);
}

void ChunksGeneratorThread::UpdateCamera(const Vector3f& position) {
    cameraPosition.Value(position);
}