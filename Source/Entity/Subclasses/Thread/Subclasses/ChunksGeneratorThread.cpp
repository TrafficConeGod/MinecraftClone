#include "ChunksGeneratorThread.h"
#include <random>

ChunksGeneratorThread::ChunksGeneratorThread(const HasChunk& vHasChunk, const CreateChunk& vCreateChunk, const RemoveChunk& vRemoveChunk, const GenerateChunkMeshes& vGenerateChunkMeshes, Chunk::Seed vSeed) : hasChunk{vHasChunk}, createChunk{vCreateChunk}, removeChunk{vRemoveChunk}, generateChunkMeshes{vGenerateChunkMeshes}, seed{vSeed} {}

void ChunksGeneratorThread::Update(float delta) {
    Vector3i currentGeneratorPosition = cameraPosition.Value();
    currentGeneratorPosition /= Chunk::Bounds;

    bool generated = false;
    for (int x = (currentGeneratorPosition.x - chunkGenerationRadius); x < (currentGeneratorPosition.x + chunkGenerationRadius); x++) {
        for (int z = (currentGeneratorPosition.z - chunkGenerationRadius); z < (currentGeneratorPosition.z + chunkGenerationRadius); z++) {
            Vector3i generatingPosition(x, currentGeneratorPosition.y, z);
            if (!hasChunk(generatingPosition)) {
                generated = true;
                GenerateChunk(generatingPosition);
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