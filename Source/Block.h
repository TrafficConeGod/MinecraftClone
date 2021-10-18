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

    static void CreateFace(ChunkGraphicsNode::Mesh& chunkMesh, const Vector3u& position, const FaceMesh& faceMesh, TextureId textureId);
};