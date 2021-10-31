#pragma once
#include <iostream>
#include "ChunkGraphicsNode.h"
#include "Vector3.h"
#include "EntityReference.h"
#include <array>

class BlockHandler;

struct Block {
    static Vector2f TexturePositionToUVCoordinate(const Vector2f& texturePosition);

    static constexpr uint Faces = 7;
    enum class Face : u_char {
        None,
        Front,
        Back,
        Top,
        Bottom,
        Right,
        Left
    };
    
    using FaceId = u_char;
    static constexpr FaceId FaceIdFor(Face face) {
        return (FaceId)face;
    }


    static constexpr uint Types = 6;
    enum class Type : u_char {
        Air,
        Grass,
        Stone,
        Dirt,
        Water,
        Sand
    };
    Type type = Type::Air;

    struct FaceTriangle {
        std::array<u_char, 3> vertexIds;
    };

    struct FaceMesh {
        std::vector<FaceTriangle> triangles;

        FaceMesh(uint meshId, uint triangleCount);
    };

    static void CreateFace(const FaceMesh& faceMesh, const Vector3u& position, const Vector2u& texturePosition, ChunkGraphicsNode::Mesh& mesh);
    const EntityReference<BlockHandler> BlockHandlerFor(const std::array<EntityReference<BlockHandler>, Block::Types>& blockHandlers) const;
};