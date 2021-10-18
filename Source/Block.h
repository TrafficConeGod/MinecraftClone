#pragma once
#include <iostream>
#include "ChunkGraphicsNode.h"
#include "Vector3.h"
#include <array>

struct Block {
    using TextureId = uint;

    static Vector2f TexturePositionToUVCoordinate(const Vector2f& texturePosition);
    static Vector2f TextureIdToTexturePosition(TextureId textureId);

    static constexpr uint Faces = 7;
    enum class Face : u_char {
        None,
        Top,
        Bottom,
        Front,
        Back,
        Left,
        Right,
    };
    
    using FaceId = u_char;
    static constexpr FaceId FaceIdFor(Face face) {
        return (FaceId)face;
    }


    static constexpr uint Types = 3;
    enum class Type : u_char {
        Air,
        Grass,
        Stone
    };
    Type type = Type::Air;

    struct FaceTriangle {
        std::array<Vector3f, 3> vertices;
        std::array<Vector2f, 3> uvVertices;
    };

    struct FaceMesh {
        std::vector<FaceTriangle> triangles;

        FaceMesh(const std::vector<FaceTriangle>& triangles);
    };

    class Mesh {
        public:
            struct FaceReference {
                using TriangleReference = ChunkGraphicsNode::Mesh::TriangleReference;

                TriangleReference triangleReference;
                std::size_t trianglesCount;
            };
        private:
            std::array<std::shared_ptr<FaceReference>, Faces> faceReferences;

            void FaceReferenceFor(Face face, std::shared_ptr<FaceReference> faceReference);
            FaceReference FaceReferenceFor(Face face);
        public:
            void CreateFace(Face face, ChunkGraphicsNode::Mesh& chunkMesh, const Vector3u& position, const FaceMesh& faceMesh, TextureId textureId);
            void DeleteFace(Face face, ChunkGraphicsNode::Mesh& chunkMesh);
            bool HasFace(Face face) const;
    };
};