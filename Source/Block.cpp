#include "Block.h"
#include "BlockHandler.h"

Vector2f Block::TexturePositionToUVCoordinate(const Vector2f& texturePosition) {
    Vector2f pixel = texturePosition;
    pixel.x *= 16;
    pixel.y *= 16;
    pixel.x += 21; // i need to do this for some reason
    return Vector2f(pixel.x / 256.f, 1.f - (pixel.y / 256.f));
}

Vector2f Block::TextureIdToTexturePosition(TextureId textureId) {
    return Vector2f(textureId, 0);
}

static constexpr float Shift = 0.009f;

Block::FaceMesh::FaceMesh(uint meshId, uint triangleCount) {
    meshId *= 3;
    for (uint i = 0; i < triangleCount; i++) {
        uint triangleId = meshId + (i * 3);
        triangles.push_back({
            triangleId,
            triangleId + 1,
            triangleId + 2
        });
    }
}

void Block::CreateFace(const FaceMesh& faceMesh, const Vector3u& position, TextureId textureId, ChunkGraphicsNode::Mesh& chunkMesh) {
    auto texturePosition = TextureIdToTexturePosition(textureId);
    chunkMesh.positionTriangles.push_back({ position, position, position });
    chunkMesh.vertexIdTriangles.insert(chunkMesh.vertexIdTriangles.end(), faceMesh.triangles.begin(), faceMesh.triangles.end());
}

const EntityReference<BlockHandler> Block::BlockHandlerFor(const std::array<EntityReference<BlockHandler>, Block::Types>& blockHandlers) const {
    return blockHandlers.at((uint)type);
}