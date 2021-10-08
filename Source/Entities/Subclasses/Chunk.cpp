#include "Chunk.h"

Chunk::Chunk(const Vector3i& vPosition, const std::array<Block, Blocks>& vBlocks, EntityReference<ChunkGraphicsNode> vNode) : node{vNode}, position{vPosition}, blocks{vBlocks} {
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
    Vector3u blockPosition(0, 0, 0);
    mesh.triangles.push_back({
        mesh.CreateVertex(blockPosition.x,blockPosition.y,blockPosition.z),
        mesh.CreateVertex(blockPosition.x,blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(blockPosition.x, 1.f + blockPosition.y, 1.f + blockPosition.z),
    });
    mesh.triangles.push_back({
        mesh.CreateVertex(1.f + blockPosition.x, 1.f + blockPosition.y,blockPosition.z),
        mesh.CreateVertex(blockPosition.x,blockPosition.y,blockPosition.z),
        mesh.CreateVertex(blockPosition.x, 1.f + blockPosition.y,blockPosition.z),
    });
    mesh.triangles.push_back({
        mesh.CreateVertex(1.f + blockPosition.x,blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(blockPosition.x,blockPosition.y,blockPosition.z),
        mesh.CreateVertex(1.f + blockPosition.x,blockPosition.y,blockPosition.z),
    });
    mesh.triangles.push_back({
        mesh.CreateVertex(1.f + blockPosition.x, 1.f + blockPosition.y,blockPosition.z),
        mesh.CreateVertex(1.f + blockPosition.x,blockPosition.y,blockPosition.z),
        mesh.CreateVertex(blockPosition.x,blockPosition.y,blockPosition.z),
    });
    mesh.triangles.push_back({
        mesh.CreateVertex(blockPosition.x,blockPosition.y,blockPosition.z),
        mesh.CreateVertex(blockPosition.x, 1.f + blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(blockPosition.x, 1.f + blockPosition.y,blockPosition.z),
    });
    mesh.triangles.push_back({
        mesh.CreateVertex(1.f + blockPosition.x,blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(blockPosition.x,blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(blockPosition.x,blockPosition.y,blockPosition.z),
    });
    mesh.triangles.push_back({
        mesh.CreateVertex(blockPosition.x, 1.f + blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(blockPosition.x,blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(1.f + blockPosition.x,blockPosition.y, 1.f + blockPosition.z),
    });
    mesh.triangles.push_back({
        mesh.CreateVertex(1.f + blockPosition.x, 1.f + blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(1.f + blockPosition.x,blockPosition.y,blockPosition.z),
        mesh.CreateVertex(1.f + blockPosition.x, 1.f + blockPosition.y,blockPosition.z),
    });
    mesh.triangles.push_back({
        mesh.CreateVertex(1.f + blockPosition.x,blockPosition.y,blockPosition.z),
        mesh.CreateVertex(1.f + blockPosition.x, 1.f + blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(1.f + blockPosition.x,blockPosition.y, 1.f + blockPosition.z),
    });
    mesh.triangles.push_back({
        mesh.CreateVertex(1.f + blockPosition.x, 1.f + blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(1.f + blockPosition.x, 1.f + blockPosition.y,blockPosition.z),
        mesh.CreateVertex(blockPosition.x, 1.f + blockPosition.y,blockPosition.z),
    });
    mesh.triangles.push_back({
        mesh.CreateVertex(1.f + blockPosition.x, 1.f + blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(blockPosition.x, 1.f + blockPosition.y,blockPosition.z),
        mesh.CreateVertex(blockPosition.x, 1.f + blockPosition.y, 1.f + blockPosition.z),
    });
    mesh.triangles.push_back({
        mesh.CreateVertex(1.f + blockPosition.x, 1.f + blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(blockPosition.x, 1.f + blockPosition.y, 1.f + blockPosition.z),
        mesh.CreateVertex(1.f + blockPosition.x,blockPosition.y, 1.f + blockPosition.z)
    });
    
    mesh.uvTriangles.push_back({
        Vector2f(0.000059f, 1.0f-0.000004f),
        Vector2f(0.000103f, 1.0f-0.336048f),
        Vector2f(0.335973f, 1.0f-0.335903f),
    });
    mesh.uvTriangles.push_back({
        Vector2f(1.000023f, 1.0f-0.000013f),
        Vector2f(0.667979f, 1.0f-0.335851f),
        Vector2f(0.999958f, 1.0f-0.336064f),
    });
    mesh.uvTriangles.push_back({
        Vector2f(0.667979f, 1.0f-0.335851f),
        Vector2f(0.336024f, 1.0f-0.671877f),
        Vector2f(0.667969f, 1.0f-0.671889f),
    });
    mesh.uvTriangles.push_back({
        Vector2f(1.000023f, 1.0f-0.000013f),
        Vector2f(0.668104f, 1.0f-0.000013f),
        Vector2f(0.667979f, 1.0f-0.335851f),
    });
    mesh.uvTriangles.push_back({
        Vector2f(0.000059f, 1.0f-0.000004f),
        Vector2f(0.335973f, 1.0f-0.335903f),
        Vector2f(0.336098f, 1.0f-0.000071f),
    });
    mesh.uvTriangles.push_back({
        Vector2f(0.667979f, 1.0f-0.335851f),
        Vector2f(0.335973f, 1.0f-0.335903f),
        Vector2f(0.336024f, 1.0f-0.671877f),
    });
    mesh.uvTriangles.push_back({
        Vector2f(1.000004f, 1.0f-0.671847f),
        Vector2f(0.999958f, 1.0f-0.336064f),
        Vector2f(0.667979f, 1.0f-0.335851f),
    });
    mesh.uvTriangles.push_back({
        Vector2f(0.668104f, 1.0f-0.000013f),
        Vector2f(0.335973f, 1.0f-0.335903f),
        Vector2f(0.667979f, 1.0f-0.335851f),
    });
    mesh.uvTriangles.push_back({
        Vector2f(0.335973f, 1.0f-0.335903f),
        Vector2f(0.668104f, 1.0f-0.000013f),
        Vector2f(0.336098f, 1.0f-0.000071f),
    });
    mesh.uvTriangles.push_back({
        Vector2f(0.000103f, 1.0f-0.336048f),
        Vector2f(0.000004f, 1.0f-0.671870f),
        Vector2f(0.336024f, 1.0f-0.671877f),
    });
    mesh.uvTriangles.push_back({
        Vector2f(0.000103f, 1.0f-0.336048f),
        Vector2f(0.336024f, 1.0f-0.671877f),
        Vector2f(0.335973f, 1.0f-0.335903f),
    });
    mesh.uvTriangles.push_back({
        Vector2f(0.667969f, 1.0f-0.671889f),
        Vector2f(1.000004f, 1.0f-0.671847f),
        Vector2f(0.667979f, 1.0f-0.335851f)
    });
}

void Chunk::RegenerateMeshAt(Mesh& mesh, const Vector3u& position) {
    
}