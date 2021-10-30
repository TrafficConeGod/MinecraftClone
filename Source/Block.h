#pragma once
#include <iostream>
#include "ChunkGraphicsNode.h"
#include "Vector3.h"
#include "EntityReference.h"
#include <array>

class BlockHandler;

struct Block {
    using TextureId = uint;

    static Vector2f TexturePositionToUVCoordinate(const Vector2f& texturePosition);
    static Vector2u TextureIdToTexturePosition(TextureId textureId);

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


    static constexpr uint Types = 4;
    enum class Type : u_char {
        Air,
        Grass,
        Stone,
        Dirt
    };
    Type type = Type::Air;

    struct FaceTriangle {
        std::array<u_char, 3> vertexIds;
    };

    struct FaceMesh {
        std::vector<FaceTriangle> triangles;

        FaceMesh(uint meshId, uint triangleCount);
    };

    static void CreateFace(const FaceMesh& faceMesh, const Vector3u& position, TextureId textureId, ChunkGraphicsNode::Mesh& chunkMesh);
    const EntityReference<BlockHandler> BlockHandlerFor(const std::array<EntityReference<BlockHandler>, Block::Types>& blockHandlers) const;
};