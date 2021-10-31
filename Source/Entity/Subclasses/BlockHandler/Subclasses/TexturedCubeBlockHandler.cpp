#include "TexturedCubeBlockHandler.h"
#include <exception>

bool TexturedCubeBlockHandler::HasMesh(const Block&) const {
    return true;
}

bool TexturedCubeBlockHandler::IsTransparent(const Block&, const Block&) const {
    return false;
}

const std::array<Block::FaceMesh, 6> faceMeshes = {
    Block::FaceMesh(0, 2),
    Block::FaceMesh(2, 2),
    Block::FaceMesh(4, 2),
    Block::FaceMesh(6, 2),
    Block::FaceMesh(8, 2),
    Block::FaceMesh(10, 2)
};

void TexturedCubeBlockHandler::GenerateFaceMesh(ChunkGraphicsNode::MeshGroup& meshes, const Vector3u& position, const Block& block, Block::Face face) const {
    if (face == Block::Face::None) {
        return;
    }

    Block::CreateFace(faceMeshes.at(((uint)face) - 1), position, TexturePositionFor(face), meshes.mainMesh);
}