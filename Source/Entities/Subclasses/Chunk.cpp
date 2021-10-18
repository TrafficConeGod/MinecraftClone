#include "Chunk.h"

Chunk::Chunk(const Vector3i& vPosition, const std::array<Block, Blocks>& vBlocks, const std::array<EntityReference<BlockHandler>, Block::Types>& vBlockHandlers, EntityReference<ChunkGraphicsNode> vNode) : node{vNode}, position{vPosition}, blocks{vBlocks}, blockHandlers{vBlockHandlers} {
    Vector3f nodePosition = position;
    nodePosition *= Bounds;
    nodePosition = Vector3f(nodePosition.x - OffsetToCenter, nodePosition.y - OffsetToCenter, nodePosition.z - OffsetToCenter);
    node->Position(nodePosition);
    node->UseMesh(std::bind(&Chunk::GenerateMesh, this, std::placeholders::_1));
}

Vector3i Chunk::ChunkPositionToWorldPosition(const Vector3i& chunkPosition, const Vector3i& localPosition) {
    return ((chunkPosition * Bounds) + localPosition);
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

EntityReference<BlockHandler> Chunk::BlockHandlerFor(Block::Type type) {
    auto index = (uint)type;
    return blockHandlers.at(index);
}

void Chunk::SetBlock(const Vector3u& position, const Block& block) {
    blocks.at(PositionToIndex(position)) = block;
    node->UseMesh(std::bind(&Chunk::RegenerateMeshAt, this, std::placeholders::_1, position));
}

void Chunk::Update() {
    for (auto& blockEntity : blockEntities) {
        blockEntity->Update();
    }
}

bool Chunk::GenerateFaceMesh(const Vector3i& direction, Block::Face face, const EntityReference<BlockHandler> blockHandler, Mesh& chunkMesh, const Vector3u& position, const Block& block) {
    auto& blockMesh = blockMeshes.at(PositionToIndex(position));
    auto checkPosition = (Vector3i)position + direction;
    if (checkPosition.x >= 0 && checkPosition.y >= 0 && checkPosition.z >= 0 && checkPosition.x < Bounds && checkPosition.y < Bounds && checkPosition.z < Bounds) {

        int checkIndex = PositionToIndex(checkPosition);
        const auto& checkBlock = blocks.at(checkIndex);
        const auto checkBlockHandler = BlockHandlerFor(checkBlock.type);

        if (checkBlockHandler->IsTransparent(checkBlock, block)) {
            blockHandler->GenerateFaceMesh(chunkMesh, blockMesh, position, block, face);
            return true;
        }
        return false;
    } else {
        blockHandler->GenerateFaceMesh(chunkMesh, blockMesh, position, block, face);
        return true;
    }
}

void Chunk::GenerateMesh(Mesh& mesh) {
    // temporary code
    std::size_t index = 0;
    for (const auto& block : blocks) {
        const auto blockHandler = BlockHandlerFor(block.type);
        Vector3u position = IndexToPosition(index);
        
        GenerateFaceMesh(Vector3i(1, 0, 0), Block::Face::Front, blockHandler, mesh, position, block);
        GenerateFaceMesh(Vector3i(-1, 0, 0), Block::Face::Back, blockHandler, mesh, position, block);
        GenerateFaceMesh(Vector3i(0, 1, 0), Block::Face::Top, blockHandler, mesh, position, block);
        GenerateFaceMesh(Vector3i(0, -1, 0), Block::Face::Bottom, blockHandler, mesh, position, block);
        GenerateFaceMesh(Vector3i(0, 0, 1), Block::Face::Right, blockHandler, mesh, position, block);
        GenerateFaceMesh(Vector3i(0, 0, -1), Block::Face::Left, blockHandler, mesh, position, block);

        index++;
    }
}

void Chunk::RegenerateMeshAt(Mesh& mesh, const Vector3u& position) {
    
}