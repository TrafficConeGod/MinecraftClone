#include "Block.h"
#include "BlockHandler.h"

Vector2f Block::TexturePositionToUVCoordinate(const Vector2f& texturePosition) {
    Vector2f pixel = texturePosition;
    pixel.x *= 16;
    pixel.y *= 16;
    pixel.x += 21; // i need to do this for some reason
    return Vector2f(pixel.x / 256.f, 1.f - (pixel.y / 256.f));
}

Vector2u Block::TextureIdToTexturePosition(TextureId textureId) {
    return Vector2u(textureId, 0);
}

static constexpr float Shift = 0.009f;

Block::FaceMesh::FaceMesh(uint meshId, uint triangleCount) {
    meshId *= 3;
    for (uint i = 0; i < triangleCount; i++) {
        uint triangleId = meshId + (i * 3);
        triangles.push_back({
            (u_char)(triangleId),
            (u_char)(triangleId + 1),
            (u_char)(triangleId + 2)
        });
    }
}

ChunkGraphicsNode::Mesh::Vertex EncodeToVertex(u_char vertexId, const Vector3u& position, const Vector2u& texturePosition) {
    return (vertexId << 0x18u) | (position.x << 0x14u) | (position.y << 0x10u) | (position.z << 0xcu) | (texturePosition.x << 0x8u) | (texturePosition.y << 0x4u);
}

void Block::CreateFace(const FaceMesh& faceMesh, const Vector3u& position, TextureId textureId, ChunkGraphicsNode::Mesh& chunkMesh) {
    auto texturePosition = TextureIdToTexturePosition(textureId);
    for (const auto& triangle : faceMesh.triangles) {
        chunkMesh.triangles.push_back({
            EncodeToVertex(triangle.vertexIds.at(0), position, texturePosition),
            EncodeToVertex(triangle.vertexIds.at(1), position, texturePosition),
            EncodeToVertex(triangle.vertexIds.at(2), position, texturePosition)
        });
    }
}

const EntityReference<BlockHandler> Block::BlockHandlerFor(const std::array<EntityReference<BlockHandler>, Block::Types>& blockHandlers) const {
    return blockHandlers.at((uint)type);
}