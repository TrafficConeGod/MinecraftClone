#include "TexturedCubeBlockHandler.h"
#include <exception>

Block::TextureId TexturedCubeBlockHandler::TextureIdFor(Block::Face face) const {
    throw std::runtime_error("Cannot call non described virtual function TextureIdFor");
}

bool TexturedCubeBlockHandler::IsTransparent(const Block&, const Block&) const {
    return false;
}

const Block::FaceMesh frontFaceMesh({
    {
        {
            Vector3f(1, 1, 1),
            Vector3f(1, 1, 0),
            Vector3f(1, 0, 0),
        },
        {
            Vector2f(0, 0),
            Vector2f(1, 0),
            Vector2f(1, 1),
        }
    },
    {
        {
            Vector3f(1, 1, 1),
            Vector3f(1, 0, 1),
            Vector3f(1, 0, 0),
        },
        {
            Vector2f(0, 0),
            Vector2f(0, 1),
            Vector2f(1, 1),
        }
    },
});

const Block::FaceMesh backFaceMesh({
    {
        {
            Vector3f(0, 1, 1),
            Vector3f(0, 1, 0),
            Vector3f(0, 0, 0),
        },
        {
            Vector2f(1, 0),
            Vector2f(0, 0),
            Vector2f(0, 1),
        }
    },
    {
        {
            Vector3f(0, 1, 1),
            Vector3f(0, 0, 1),
            Vector3f(0, 0, 0),
        },
        {
            Vector2f(1, 0),
            Vector2f(1, 1),
            Vector2f(0, 1),
        }
    },
});

const Block::FaceMesh topFaceMesh({
    {
        {
            Vector3f(1, 1, 1),
            Vector3f(1, 1, 0),
            Vector3f(0, 1, 0),
        },
        {
            Vector2f(1, 1),
            Vector2f(1, 0),
            Vector2f(0, 0),
        }
    },
    {
        {
            Vector3f(1, 1, 1),
            Vector3f(0, 1, 1),
            Vector3f(0, 1, 0),
        },
        {
            Vector2f(1, 1),
            Vector2f(0, 1),
            Vector2f(0, 0),
        }
    },
});

const Block::FaceMesh bottomFaceMesh({
    {
        {
            Vector3f(1, 0, 1),
            Vector3f(1, 0, 0),
            Vector3f(0, 0, 0),
        },
        {
            Vector2f(1, 1),
            Vector2f(1, 0),
            Vector2f(0, 0),
        }
    },
    {
        {
            Vector3f(1, 0, 1),
            Vector3f(0, 0, 1),
            Vector3f(0, 0, 0),
        },
        {
            Vector2f(1, 1),
            Vector2f(0, 1),
            Vector2f(0, 0),
        }
    },
});

const Block::FaceMesh rightFaceMesh({
    {
        {
            Vector3f(1, 1, 1),
            Vector3f(0, 1, 1),
            Vector3f(0, 0, 1),
        },
        {
            Vector2f(1, 0),
            Vector2f(0, 0),
            Vector2f(0, 1),
        }
    },
    {
        {
            Vector3f(1, 1, 1),
            Vector3f(1, 0, 1),
            Vector3f(0, 0, 1),
        },
        {
            Vector2f(1, 0),
            Vector2f(1, 1),
            Vector2f(0, 1),
        }
    },
});

const Block::FaceMesh leftFaceMesh({
    {
        {
            Vector3f(1, 1, 0),
            Vector3f(0, 1, 0),
            Vector3f(0, 0, 0),
        },
        {
            Vector2f(0, 0),
            Vector2f(1, 0),
            Vector2f(1, 1),
        }
    },
    {
        {
            Vector3f(1, 1, 0),
            Vector3f(1, 0, 0),
            Vector3f(0, 0, 0),
        },
        {
            Vector2f(0, 0),
            Vector2f(0, 1),
            Vector2f(1, 1),
        }
    },
});

void TexturedCubeBlockHandler::GenerateFaceMesh(ChunkGraphicsNode::Mesh& chunkMesh, Block::Mesh& blockMesh, const Vector3u& position, const Block& block, Block::Face face) const {
    auto textureId = TextureIdFor(face);
    switch (face) {
        case Block::Face::Front: {
            blockMesh.CreateFace(face, chunkMesh, position, frontFaceMesh, textureId);
        } break;
        case Block::Face::Back: {
            blockMesh.CreateFace(face, chunkMesh, position, backFaceMesh, textureId);
        } break;
        case Block::Face::Top: {
            blockMesh.CreateFace(face, chunkMesh, position, topFaceMesh, textureId);
        } break;
        case Block::Face::Bottom: {
            blockMesh.CreateFace(face, chunkMesh, position, bottomFaceMesh, textureId);
        } break;
        case Block::Face::Left: {
            blockMesh.CreateFace(face, chunkMesh, position, leftFaceMesh, textureId);
        } break;
        case Block::Face::Right: {
            blockMesh.CreateFace(face, chunkMesh, position, rightFaceMesh, textureId);
        } break;
        default: break;
    }
}