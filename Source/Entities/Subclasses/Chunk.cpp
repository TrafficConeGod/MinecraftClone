#include "Chunk.h"
#include "Mod.h"
#include "PerlinNoise.hpp"

Chunk::Chunk(const IsBlockAtWorldPositionTransparent& vIsBlockAtWorldPositionTransparent, const std::array<EntityReference<BlockHandler>, Block::Types>& vBlockHandlers, EntityReference<ChunkGraphicsNode> vNode, const Vector3i& vPosition) : isBlockAtWorldPositionTransparent{vIsBlockAtWorldPositionTransparent}, blockHandlers{vBlockHandlers}, node{vNode}, position{vPosition} {
    Vector3f nodePosition = position;
    nodePosition *= Bounds;
    node->Position(nodePosition);
}

void Chunk::GenerateBlocks(Seed seed) {
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
                    BlockAt(localPosition, { Block::Type::Grass });
                } else if (worldPosition.y < yPos) {
                    if ((worldPosition.y - yPos) > -3) {
                        BlockAt(localPosition, { Block::Type::Dirt });
                    } else {
                        BlockAt(localPosition, { Block::Type::Stone });
                    }
                }
            }
        }
    }
}

Vector3i Chunk::FreePositionToGridPosition(const Vector3f& freePosition) {
    return Vector3i(std::floor(freePosition.x), std::floor(freePosition.y), std::floor(freePosition.z));
}

Vector3i Chunk::LocalChunkPositionToWorldPosition(const Vector3i& chunkPosition, const Vector3u& localPosition) {
    return ((chunkPosition * Bounds) + (Vector3i)localPosition);
}

Vector3i Chunk::WorldPositionToChunkPosition(const Vector3f& worldPosition) {
    return FreePositionToGridPosition((Vector3f)FreePositionToGridPosition(worldPosition) / (float)Bounds);
}

Vector3u Chunk::WorldPositionToLocalChunkPosition(const Vector3f& worldPosition) {
    Vector3f roundedWorldPosition = FreePositionToGridPosition(worldPosition);
    return FreePositionToGridPosition(Vector3f(Mod(roundedWorldPosition.x, Bounds), Mod(roundedWorldPosition.y, Bounds), Mod(roundedWorldPosition.z, Bounds)));
}

std::size_t Chunk::PositionToIndex(const Vector3i& position) {
    return position.x + (position.y * Bounds) + (position.z * Bounds * Bounds);
}

// from https://stackoverflow.com/questions/7367770/how-to-flatten-or-index-3d-array-in-1d-array
Vector3u Chunk::IndexToPosition(std::size_t index) {
    uint z = index / (Bounds * Bounds);
    index -= (z * Bounds * Bounds);
    uint y = index / Bounds;
    uint x = index % Bounds;
    return Vector3u(x, y, z);
}

const Block& Chunk::BlockAt(const Vector3u& position) const {
    return blocks.at(PositionToIndex(position));
}

const Block& Chunk::BlockAt(std::size_t index) const {
    return blocks.at(index);
}

void Chunk::BlockAt(const Vector3u& position, const Block& block) {
    blocks.at(PositionToIndex(position)) = block;
}

bool Chunk::IsBlockAtLocalPositionTransparent(const Vector3u& position, const Block& neighborBlock) const {
    const auto& block = BlockAt(position);
    auto blockHandler = block.BlockHandlerFor(blockHandlers);
    return blockHandler->IsTransparent(block, neighborBlock);
}

void Chunk::UpdateMesh() {
    node->UseMesh([&](auto& mesh) {
        mesh.triangles.clear();
        mesh.uvTriangles.clear();
        GenerateMesh(mesh);
    });
}

void Chunk::Update() {
    for (auto blockEntity : blockEntities) {
        blockEntity->Update();
    }
}

bool Chunk::GenerateFaceMesh(const Vector3i& direction, Block::Face face, const EntityReference<BlockHandler> blockHandler, Mesh& chunkMesh, const Vector3u& position, const Block& block) {
    auto checkPosition = (Vector3i)position + direction;
    if (checkPosition.x >= 0 && checkPosition.y >= 0 && checkPosition.z >= 0 && checkPosition.x < Bounds && checkPosition.y < Bounds && checkPosition.z < Bounds) {

        int checkIndex = PositionToIndex(checkPosition);
        const auto& checkBlock = BlockAt(checkIndex);
        const auto checkBlockHandler = checkBlock.BlockHandlerFor(blockHandlers);

        if (checkBlockHandler->IsTransparent(checkBlock, block)) {
            blockHandler->GenerateFaceMesh(chunkMesh, position, block, face);
            return true;
        }
        return false;
    } else if (isBlockAtWorldPositionTransparent(LocalChunkPositionToWorldPosition(this->position, checkPosition), block)) {
        blockHandler->GenerateFaceMesh(chunkMesh, position, block, face);
        return true;
    }
    return false;
}

void Chunk::GenerateMesh(Mesh& mesh) {
    std::size_t index = 0;
    for (const auto& block : blocks) {
        const auto blockHandler = block.BlockHandlerFor(blockHandlers);
        Vector3u position = IndexToPosition(index);
        
        bool faceGenerated = false;
        faceGenerated |= GenerateFaceMesh(Vector3i(1, 0, 0), Block::Face::Front, blockHandler, mesh, position, block);
        faceGenerated |= GenerateFaceMesh(Vector3i(-1, 0, 0), Block::Face::Back, blockHandler, mesh, position, block);
        faceGenerated |= GenerateFaceMesh(Vector3i(0, 1, 0), Block::Face::Top, blockHandler, mesh, position, block);
        faceGenerated |= GenerateFaceMesh(Vector3i(0, -1, 0), Block::Face::Bottom, blockHandler, mesh, position, block);
        faceGenerated |= GenerateFaceMesh(Vector3i(0, 0, 1), Block::Face::Right, blockHandler, mesh, position, block);
        faceGenerated |= GenerateFaceMesh(Vector3i(0, 0, -1), Block::Face::Left, blockHandler, mesh, position, block);

        if (faceGenerated) {
            blockHandler->GenerateFaceMesh(mesh, position, block, Block::Face::None);
        }

        index++;
    }
}