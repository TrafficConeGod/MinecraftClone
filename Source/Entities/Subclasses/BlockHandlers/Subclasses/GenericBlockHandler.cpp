#include "GenericBlockHandler.h"

GenericBlockHandler::GenericBlockHandler(Block::Type vType) : type{vType} {}

bool GenericBlockHandler::IsTransparent(const Block&, const Block&) const {
    return false;
}

void GenericBlockHandler::GenerateFaceMesh(ChunkGraphicsNode::Mesh& mesh, const Vector3u& position, const Block& block, Block::Face face) const {
    switch (face) {
        case Block::Face::Front: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(1.f + position.x, 1.f + position.y, 1.f + position.z),
                mesh.CreateVertex(1.f + position.x,position.y,position.z),
                mesh.CreateVertex(1.f + position.x, 1.f + position.y,position.z),
            });
            mesh.uvTriangles.push_back({ 
                Vector2f(0.668104f, 1.0f-0.000013f),
                Vector2f(0.335973f, 1.0f-0.335903f),
                Vector2f(0.667979f, 1.0f-0.335851f),
            });
            mesh.triangles.push_back({
                mesh.CreateVertex(1.f + position.x,position.y,position.z),
                mesh.CreateVertex(1.f + position.x, 1.f + position.y, 1.f + position.z),
                mesh.CreateVertex(1.f + position.x,position.y, 1.f + position.z),
            });
            mesh.uvTriangles.push_back({
                Vector2f(0.335973f, 1.0f-0.335903f),
                Vector2f(0.668104f, 1.0f-0.000013f),
                Vector2f(0.336098f, 1.0f-0.000071f),
            });
        } break;
        case Block::Face::Back: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x,position.y,position.z),
                mesh.CreateVertex(position.x,position.y, 1.f + position.z),
                mesh.CreateVertex(position.x, 1.f + position.y, 1.f + position.z),
            });
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x,position.y,position.z),
                mesh.CreateVertex(position.x, 1.f + position.y, 1.f + position.z),
                mesh.CreateVertex(position.x, 1.f + position.y,position.z),
            });
            mesh.uvTriangles.push_back({ 
                Vector2f(0.000059f, 1.0f-0.000004f),
                Vector2f(0.000103f, 1.0f-0.336048f),
                Vector2f(0.335973f, 1.0f-0.335903f),
            });
            mesh.uvTriangles.push_back({ 
                Vector2f(0.000059f, 1.0f-0.000004f),
                Vector2f(0.335973f, 1.0f-0.335903f),
                Vector2f(0.336098f, 1.0f-0.000071f),
            });
        } break;
        case Block::Face::Top: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(1.f + position.x, 1.f + position.y, 1.f + position.z),
                mesh.CreateVertex(1.f + position.x, 1.f + position.y,position.z),
                mesh.CreateVertex(position.x, 1.f + position.y,position.z),
            });
            mesh.uvTriangles.push_back({ 
                Vector2f(0.000103f, 1.0f-0.336048f),
                Vector2f(0.000004f, 1.0f-0.671870f),
                Vector2f(0.336024f, 1.0f-0.671877f),
            });
            mesh.triangles.push_back({
                mesh.CreateVertex(1.f + position.x, 1.f + position.y, 1.f + position.z),
                mesh.CreateVertex(position.x, 1.f + position.y,position.z),
                mesh.CreateVertex(position.x, 1.f + position.y, 1.f + position.z),
            });
            
        
            mesh.uvTriangles.push_back({
                Vector2f(0.000103f, 1.0f-0.336048f),
                Vector2f(0.336024f, 1.0f-0.671877f),
                Vector2f(0.335973f, 1.0f-0.335903f),
            });
        } break;
        case Block::Face::Bottom: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(1.f + position.x,position.y, 1.f + position.z),
                mesh.CreateVertex(position.x,position.y,position.z),
                mesh.CreateVertex(1.f + position.x,position.y,position.z),
            });
            mesh.triangles.push_back({ 
                mesh.CreateVertex(1.f + position.x,position.y, 1.f + position.z),
                mesh.CreateVertex(position.x,position.y, 1.f + position.z),
                mesh.CreateVertex(position.x,position.y,position.z),
            });
            mesh.uvTriangles.push_back({ 
                Vector2f(0.667979f, 1.0f-0.335851f),
                Vector2f(0.336024f, 1.0f-0.671877f),
                Vector2f(0.667969f, 1.0f-0.671889f),
            });
            mesh.uvTriangles.push_back({ 
                Vector2f(0.667979f, 1.0f-0.335851f),
                Vector2f(0.335973f, 1.0f-0.335903f),
                Vector2f(0.336024f, 1.0f-0.671877f),
            });
        } break;
        case Block::Face::Right: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(position.x, 1.f + position.y, 1.f + position.z),
                mesh.CreateVertex(position.x,position.y, 1.f + position.z),
                mesh.CreateVertex(1.f + position.x,position.y, 1.f + position.z),
            });
            mesh.triangles.push_back({ 
                mesh.CreateVertex(1.f + position.x, 1.f + position.y, 1.f + position.z),
                mesh.CreateVertex(position.x, 1.f + position.y, 1.f + position.z),
                mesh.CreateVertex(1.f + position.x,position.y, 1.f + position.z)
            });

            mesh.uvTriangles.push_back({ 
                Vector2f(1.000004f, 1.0f-0.671847f),
                Vector2f(0.999958f, 1.0f-0.336064f),
                Vector2f(0.667979f, 1.0f-0.335851f),
            });
            mesh.uvTriangles.push_back({ 
                Vector2f(0.667969f, 1.0f-0.671889f),
                Vector2f(1.000004f, 1.0f-0.671847f),
                Vector2f(0.667979f, 1.0f-0.335851f)
            });
        } break;
        case Block::Face::Left: {
            mesh.triangles.push_back({ 
                mesh.CreateVertex(1.f + position.x, 1.f + position.y,position.z),
                mesh.CreateVertex(position.x,position.y,position.z),
                mesh.CreateVertex(position.x, 1.f + position.y,position.z),
            });
            mesh.triangles.push_back({ 
                mesh.CreateVertex(1.f + position.x, 1.f + position.y,position.z),
                mesh.CreateVertex(1.f + position.x,position.y,position.z),
                mesh.CreateVertex(position.x,position.y,position.z),
            });
            mesh.uvTriangles.push_back({ 
                Vector2f(1.000023f, 1.0f-0.000013f),
                Vector2f(0.667979f, 1.0f-0.335851f),
                Vector2f(0.999958f, 1.0f-0.336064f),
            });
            mesh.uvTriangles.push_back({ 
                Vector2f(1.000023f, 1.0f-0.000013f),
                Vector2f(0.668104f, 1.0f-0.000013f),
                Vector2f(0.667979f, 1.0f-0.335851f),
            });
        } break;
        default: {

        } break;
    }
}