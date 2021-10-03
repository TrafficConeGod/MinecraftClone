#include "Chunk.h"

Chunk::Chunk(const Vector3u& vPosition, const std::array<Block, 4096>& vBlocks, EntityReference<GraphicsNode> vNode) : position{vPosition}, node{vNode} {
    node->Position(position);
    std::size_t index = 0;
    for (const auto& block : blocks) {
        SetBlock(index, block);
        index++;
    }
}

std::size_t Chunk::PositionToIndex(const Vector3u& position) {
    return position.x + (position.y * Bounds);
}

void Chunk::SetBlock(std::size_t index, const Block& block) {
    blocks.at(index) = block;
    // TODO: update the mesh
}

void Chunk::SetBlock(const Vector3u& position, const Block& block) {
    SetBlock(PositionToIndex(position), block);
}

void Chunk::Update() {
    std::cout << "chunk update\n";
    for (auto& blockEntity : blockEntities) {
        blockEntity->Update();
    }
}