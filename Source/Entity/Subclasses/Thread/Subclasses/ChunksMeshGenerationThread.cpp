#include "ChunksMeshGenerationThread.h"

ChunksMeshGenerationThread::ChunksMeshGenerationThread(const HasChunkAt& vHasChunkAt, const ChunkAt& vChunkAt) : hasChunkAt{vHasChunkAt}, chunkAt{vChunkAt} {}

void ChunksMeshGenerationThread::Update(float delta) {
    if (currentChunks.size() == 0 && shouldBeginGenerating) {
        chunks.Use([&](auto& chunks) {
            shouldBeginGenerating = false;
            currentChunks = chunks;
            chunks.clear();
        });
    }
    if (currentChunks.size() > 0) {
        for (auto chunk : currentChunks) {
            auto position = chunk->Position();

            auto generateChunkMeshHelper = [&](const Vector3i& direction) {
                GenerateChunkMeshAtIfNotInBatch(position + direction);
            };
            generateChunkMeshHelper(Vector3i(1, 0, 0));
            generateChunkMeshHelper(Vector3i(-1, 0, 0));
            generateChunkMeshHelper(Vector3i(0, 1, 0));
            generateChunkMeshHelper(Vector3i(0, -1, 0));
            generateChunkMeshHelper(Vector3i(0, 0, 1));
            generateChunkMeshHelper(Vector3i(0, 0, -1));
            chunk->UpdateMesh();
        }
        currentChunks.clear();
    }
}

void ChunksMeshGenerationThread::GenerateChunkMeshAtIfNotInBatch(const Vector3i& position) {
    if (hasChunkAt(position)) {
        auto chunk = chunkAt(position);
        bool canGenerate = true;
        for (auto checkChunk : currentChunks) {
            if (checkChunk == chunk) {
                canGenerate = false;
                break;
            }
        }
        if (canGenerate) {
            chunk->UpdateMesh();
        }
    }
}

void ChunksMeshGenerationThread::AddChunk(EntityReference<Chunk> chunk) {
    chunks.Use([chunk](auto& chunks) {
        chunks.push_back(chunk);
    });
}

void ChunksMeshGenerationThread::Generate() {
    shouldBeginGenerating = true;
}