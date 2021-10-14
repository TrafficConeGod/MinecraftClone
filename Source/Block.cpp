#include "Block.h"

Block::FaceMesh::FaceMesh(const std::vector<FaceTriangle>& vTriangles) : triangles{vTriangles} {}

void Block::Mesh::FaceMeshPointer(Face face, const MeshPointer& meshPointer) {}

void Block::Mesh::AddTrianglesTo(ChunkGraphicsNode::Mesh& chunkMesh, const Vector3u& position, Face face, const FaceMesh& faceMesh) {
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
            Vector2f(0, 0),
            Vector2f(0, 0.0625),
            Vector2f(0.0625, 0.0625),
        }});
    }
}