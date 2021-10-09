#include "GenericBlockHandler.h"

GenericBlockHandler::GenericBlockHandler(Block::Type vType) : type{vType} {}

bool GenericBlockHandler::IsTransparent(const Block&, const Block&) const {
    return false;
}

void GenericBlockHandler::GenerateFaceMesh(ChunkGraphicsNode::Mesh& mesh, const Vector3u& position, const Block& block, Block::Face face) const {
    uint id = (uint)type;
    Vector2u texturePosition(id, 0);

    switch (face) {
        case Block::Face::Front: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x + 1, position.y + 1, position.z + 1),
                mesh.CreateVertex(position.x + 1, position.y, position.z + 1),
                mesh.CreateVertex(position.x + 1, position.y, position.z),
            });
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x + 1, position.y + 1, position.z),
                mesh.CreateVertex(position.x + 1, position.y + 1, position.z + 1),
                mesh.CreateVertex(position.x + 1, position.y, position.z),
            });
            
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y),
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
            });
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
            });
        } break;
        case Block::Face::Back: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x, position.y, position.z),
                mesh.CreateVertex(position.x, position.y, position.z + 1),
                mesh.CreateVertex(position.x, position.y + 1, position.z + 1),
            });
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x, position.y, position.z),
                mesh.CreateVertex(position.x, position.y + 1, position.z + 1),
                mesh.CreateVertex(position.x, position.y + 1, position.z),
            });
            
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
            });
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y + 1),
            });
        } break;
        case Block::Face::Top: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x, position.y + 1, position.z),
                mesh.CreateVertex(position.x, position.y + 1, position.z + 1),
                mesh.CreateVertex(position.x + 1, position.y + 1, position.z + 1),
            });
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x, position.y + 1, position.z),
                mesh.CreateVertex(position.x + 1, position.y + 1, position.z + 1),
                mesh.CreateVertex(position.x + 1, position.y + 1, position.z),
            });
            
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
            });
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y),
            });
        } break;
        case Block::Face::Bottom: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x + 1, position.y, position.z + 1),
                mesh.CreateVertex(position.x, position.y, position.z + 1),
                mesh.CreateVertex(position.x, position.y, position.z),
            });
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x + 1, position.y, position.z),
                mesh.CreateVertex(position.x + 1, position.y, position.z + 1),
                mesh.CreateVertex(position.x, position.y, position.z),
            });
            
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
            });
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
            });
        } break;
        case Block::Face::Right: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x + 1, position.y + 1, position.z + 1),
                mesh.CreateVertex(position.x, position.y + 1, position.z + 1),
                mesh.CreateVertex(position.x, position.y, position.z + 1),
            });
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x + 1, position.y, position.z + 1),
                mesh.CreateVertex(position.x + 1, position.y + 1, position.z + 1),
                mesh.CreateVertex(position.x, position.y, position.z + 1),
            });
            
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
            });
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
            });
        } break;
        case Block::Face::Left: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x, position.y, position.z),
                mesh.CreateVertex(position.x, position.y + 1, position.z),
                mesh.CreateVertex(position.x + 1, position.y + 1, position.z),
            });
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x, position.y, position.z),
                mesh.CreateVertex(position.x + 1, position.y + 1, position.z),
                mesh.CreateVertex(position.x + 1, position.y, position.z),
            });
            
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
            });
            mesh.uvTriangles.push_back({ 
                mesh.CreateUVVertex(texturePosition.x, texturePosition.y),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y + 1),
                mesh.CreateUVVertex(texturePosition.x + 1, texturePosition.y),
            });
        } break;
        default: {

        } break;
    }
}