#include "Chunk.h"

Chunk::Chunk(const std::array<Block, 4096>& vBlocks, EntityReference<GraphicsNode> vNode) : blocks{vBlocks}, node{vNode} {}

void Chunk::Update() {
    std::cout << "chunk update\n";
    for (auto& blockEntity : blockEntities) {
        blockEntity->Update();
    }
}