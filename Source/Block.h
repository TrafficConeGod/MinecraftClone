#pragma once
#include <iostream>
#include "ChunkGraphicsNode.h"
#include "Vector3.h"
#include <array>

struct Block {
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
    };

    struct FaceMesh {
        std::vector<FaceTriangle> triangles;

        FaceMesh(const std::vector<FaceTriangle>& triangles);
    };

    class Mesh {
        private:
            struct MeshPointer {
                std::size_t index = 0;
                u_char triangles = 0;
            };

            std::array<MeshPointer, Faces> faceMeshPointers;

            void FaceMeshPointer(Face face, const MeshPointer& meshPointer);
            MeshPointer FaceMeshPointer(Face face) const;
        public:
            void AddTrianglesTo(ChunkGraphicsNode::Mesh& chunkMesh, const Vector3u& position, Face face, const FaceMesh& faceMesh);

            bool HasFace(Face face) const;
    };
};