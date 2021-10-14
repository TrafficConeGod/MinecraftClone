#include "TexturedCubeBlockHandler.h"
#include <exception>

uint TexturedCubeBlockHandler::TextureIdFor(Block::Face face) const {
    throw std::runtime_error("Cannot call non described virtual function TextureIdFor");
}

bool TexturedCubeBlockHandler::IsTransparent(const Block&, const Block&) const {
    return false;
}

Vector2u TextureIdToPosition(uint id) {
    return Vector2u(id, 0);
}

const Block::FaceMesh frontFaceMesh({
    {
        Vector3f(1, 1, 1),
        Vector3f(1, 1, 0),
        Vector3f(1, 0, 0),
    },
    {
        Vector3f(1, 1, 1),
        Vector3f(1, 0, 1),
        Vector3f(1, 0, 0),
    },
});

void TexturedCubeBlockHandler::GenerateFaceMesh(ChunkGraphicsNode::Mesh& chunkMesh, Block::Mesh& blockMesh, const Vector3u& position, const Block& block, Block::Face face) const {
    Vector2u texturePosition = TextureIdToPosition(TextureIdFor(face));

    switch (face) {
        case Block::Face::Front: {
            blockMesh.AddTrianglesTo(chunkMesh, position, face, frontFaceMesh);
        } break;
        default: break;
    }
}