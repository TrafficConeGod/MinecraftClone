#include "Chunk.h"

Chunk::Chunk(const Vector3i& vPosition, const std::array<Block, Blocks>& vBlocks, const std::array<EntityReference<BlockHandler>, BlockHandlers>& vBlockHandlers, EntityReference<ChunkGraphicsNode> vNode) : node{vNode}, position{vPosition}, blocks{vBlocks}, blockHandlers{vBlockHandlers} {
    Vector3f nodePosition = position;
    nodePosition *= Bounds;
    nodePosition = Vector3f(nodePosition.x - OffsetToCenter, nodePosition.y - OffsetToCenter, nodePosition.z - OffsetToCenter);
    node->Position(nodePosition);
    node->UseMesh(std::bind(&Chunk::GenerateMesh, this, std::placeholders::_1));
}

std::size_t Chunk::PositionToIndex(const Vector3u& position) {
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
    node->UseMesh(std::bind(&Chunk::RegenerateMeshAt, this, std::placeholders::_1, position));
}

void Chunk::Update() {
    for (auto& blockEntity : blockEntities) {
        blockEntity->Update();
    }
}

void Chunk::GenerateMesh(Mesh& mesh) {
    // temporary code
    std::size_t index = 0;
    for (const auto& block : blocks) {
        const auto blockHandler = BlockHandlerFor(block.type);
        blockHandler->GenerateFaceMesh(mesh, IndexToPosition(index), block, Block::Face::Top);
        index++;
    }
}

void Chunk::RegenerateMeshAt(Mesh& mesh, const Vector3u& position) {
    
}