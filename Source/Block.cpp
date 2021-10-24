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

Block::FaceMesh::FaceMesh(const std::vector<FaceTriangle>& vTriangles) : triangles{vTriangles} {}

void Block::CreateFace(ChunkGraphicsNode::Mesh& chunkMesh, const Vector3u& position, const FaceMesh& faceMesh, TextureId textureId) {
    auto floatPosition = (Vector3f)position;
    auto texturePosition = TextureIdToTexturePosition(textureId);
    for (const auto& triangle : faceMesh.triangles) {
        chunkMesh.triangles.push_back({{
            (triangle.vertices.at(0) + floatPosition),
            (triangle.vertices.at(1) + floatPosition),
            (triangle.vertices.at(2) + floatPosition),
        }});

        chunkMesh.uvTriangles.push_back({{
            TexturePositionToUVCoordinate(texturePosition + triangle.uvVertices.at(0)),
            TexturePositionToUVCoordinate(texturePosition + triangle.uvVertices.at(1)),
            TexturePositionToUVCoordinate(texturePosition + triangle.uvVertices.at(2)),
        }});
    }
}

const EntityReference<BlockHandler> Block::BlockHandlerFor(const std::array<EntityReference<BlockHandler>, Block::Types>& blockHandlers) const {
    return blockHandlers.at((uint)type);
}