#include "Chunk.h"

Chunk::Chunk(const Vector3i& vPosition, const std::array<Block, Blocks>& vBlocks, EntityReference<GraphicsNode> vNode) : position{vPosition}, node{vNode} {
    Vector3f nodePosition = position;
    nodePosition *= Bounds;
    nodePosition = Vector3f(nodePosition.x - OffsetToCenter, nodePosition.y - OffsetToCenter, nodePosition.z - OffsetToCenter);
    node->Position(nodePosition);
    std::size_t index = 0;
    for (const auto& block : vBlocks) {
        SetBlock(index, block);
        index++;
    }
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

void Chunk::SetBlock(std::size_t index, const Block& block) {
    blocks.at(index) = block;
    BlockMesh& blockMesh = blockMeshes.at(index);
    switch (block.type) {
        case Block::Type::Air: {
            if (blockMesh.index != 0) {
                // TODO: Handle block removal
                throw std::runtime_error("Block removal is not handled yet");
            }
        } break;
        default: {
            if (blockMesh.index == 0) {
                node->UseMesh([&](auto& mesh) {
                    blockMesh.index = mesh.triangles.size() + 1;
                    Vector3f blockPosition = IndexToPosition(index);
                    blockPosition = Vector3f(blockPosition.x + 0.5f, blockPosition.y + 0.5f, blockPosition.z + 0.5f);
                    // mesh.triangles.push_back({
                    //     Vector3f(-0.5f + blockPosition.x,-0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x,-0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    // });
                    // mesh.triangles.push_back({
                    //     Vector3f(0.5f + blockPosition.x, 0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x,-0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x, 0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    // });
                    // mesh.triangles.push_back({
                    //     Vector3f(0.5f + blockPosition.x,-0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x,-0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    //     Vector3f(0.5f + blockPosition.x,-0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    // });
                    // mesh.triangles.push_back({
                    //     Vector3f(0.5f + blockPosition.x, 0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    //     Vector3f(0.5f + blockPosition.x,-0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x,-0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    // });
                    // mesh.triangles.push_back({
                    //     Vector3f(-0.5f + blockPosition.x,-0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x, 0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    // });
                    // mesh.triangles.push_back({
                    //     Vector3f(0.5f + blockPosition.x,-0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x,-0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x,-0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    // });
                    // mesh.triangles.push_back({
                    //     Vector3f(-0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x,-0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(0.5f + blockPosition.x,-0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    // });
                    // mesh.triangles.push_back({
                    //     Vector3f(0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(0.5f + blockPosition.x,-0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    //     Vector3f(0.5f + blockPosition.x, 0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    // });
                    // mesh.triangles.push_back({
                    //     Vector3f(0.5f + blockPosition.x,-0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    //     Vector3f(0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(0.5f + blockPosition.x,-0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    // });
                    // mesh.triangles.push_back({
                    //     Vector3f(0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(0.5f + blockPosition.x, 0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x, 0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    // });
                    // mesh.triangles.push_back({
                    //     Vector3f(0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x, 0.5f + blockPosition.y,-0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    // });
                    // mesh.triangles.push_back({
                    //     Vector3f(0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(-0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z),
                    //     Vector3f(0.5f + blockPosition.x,-0.5f + blockPosition.y, 0.5f + blockPosition.z)
                    // });
                    
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(0.000059f, 1.0f-0.000004f),
                    //     Vector2f(0.000103f, 1.0f-0.336048f),
                    //     Vector2f(0.335973f, 1.0f-0.335903f),
                    // });
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(1.000023f, 1.0f-0.000013f),
                    //     Vector2f(0.667979f, 1.0f-0.335851f),
                    //     Vector2f(0.999958f, 1.0f-0.336064f),
                    // });
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(0.667979f, 1.0f-0.335851f),
                    //     Vector2f(0.336024f, 1.0f-0.671877f),
                    //     Vector2f(0.667969f, 1.0f-0.671889f),
                    // });
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(1.000023f, 1.0f-0.000013f),
                    //     Vector2f(0.668104f, 1.0f-0.000013f),
                    //     Vector2f(0.667979f, 1.0f-0.335851f),
                    // });
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(0.000059f, 1.0f-0.000004f),
                    //     Vector2f(0.335973f, 1.0f-0.335903f),
                    //     Vector2f(0.336098f, 1.0f-0.000071f),
                    // });
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(0.667979f, 1.0f-0.335851f),
                    //     Vector2f(0.335973f, 1.0f-0.335903f),
                    //     Vector2f(0.336024f, 1.0f-0.671877f),
                    // });
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(1.000004f, 1.0f-0.671847f),
                    //     Vector2f(0.999958f, 1.0f-0.336064f),
                    //     Vector2f(0.667979f, 1.0f-0.335851f),
                    // });
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(0.668104f, 1.0f-0.000013f),
                    //     Vector2f(0.335973f, 1.0f-0.335903f),
                    //     Vector2f(0.667979f, 1.0f-0.335851f),
                    // });
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(0.335973f, 1.0f-0.335903f),
                    //     Vector2f(0.668104f, 1.0f-0.000013f),
                    //     Vector2f(0.336098f, 1.0f-0.000071f),
                    // });
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(0.000103f, 1.0f-0.336048f),
                    //     Vector2f(0.000004f, 1.0f-0.671870f),
                    //     Vector2f(0.336024f, 1.0f-0.671877f),
                    // });
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(0.000103f, 1.0f-0.336048f),
                    //     Vector2f(0.336024f, 1.0f-0.671877f),
                    //     Vector2f(0.335973f, 1.0f-0.335903f),
                    // });
                    // mesh.uvTriangles.push_back({
                    //     Vector2f(0.667969f, 1.0f-0.671889f),
                    //     Vector2f(1.000004f, 1.0f-0.671847f),
                    //     Vector2f(0.667979f, 1.0f-0.335851f)
                    // });
                });
            }
        } break;
    }
}

void Chunk::SetBlock(const Vector3u& position, const Block& block) {
    SetBlock(PositionToIndex(position), block);
}

void Chunk::Update() {
    // std::cout << "chunk update\n";
    for (auto& blockEntity : blockEntities) {
        blockEntity->Update();
    }
}