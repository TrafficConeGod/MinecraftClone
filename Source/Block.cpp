#include "Block.h"

Vector2f Block::TexturePositionToUVCoordinate(const Vector2f& texturePosition) {
    Vector2f pixel = texturePosition;
    pixel.x *= 16;
    pixel.y *= 16;
    pixel.x += 21; // i need to do this for some reason
    return Vector2f(pixel.x / 256.f, 1.f - (pixel.y / 256.f));
}

Block::FaceMesh::FaceMesh(const std::vector<FaceTriangle>& vTriangles) : triangles{vTriangles} {}

void Block::Mesh::FaceMeshPointer(Face face, const MeshPointer& meshPointer) {}

void Block::Mesh::AddTrianglesTo(ChunkGraphicsNode::Mesh& chunkMesh, const Vector3u& position, Face face, const FaceMesh& faceMesh, TextureId textureId) {
    FaceMeshPointer(face, MeshPointer{ chunkMesh.triangles.size() - 1, (u_char)faceMesh.triangles.size() });
    
    auto floatPosition = (Vector3f)position;
    for (const auto& triangle : faceMesh.triangles) {
        chunkMesh.triangles.push_back({{
            (triangle.vertices.at(0) + floatPosition),
            (triangle.vertices.at(1) + floatPosition),
            (triangle.vertices.at(2) + floatPosition),
        }});

        // TODO: Implement proper UV
        chunkMesh.uvTriangles.push_back({{
            TexturePositionToUVCoordinate(Vector2u(0, 0)),
            TexturePositionToUVCoordinate(Vector2u(1, 0)),
            TexturePositionToUVCoordinate(Vector2u(1, 1)),
        }});
    }
}