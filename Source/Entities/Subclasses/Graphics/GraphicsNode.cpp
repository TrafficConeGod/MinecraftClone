#include "GraphicsNode.h"

using RenderMesh = GraphicsNode::RenderMesh;

GraphicsNode::GraphicsNode(GLuint vVertexBuffer, GLuint vUvBuffer, const RenderMesh& vMesh) : Entity(), vertexBuffer{vVertexBuffer}, uvBuffer{vUvBuffer}, mesh{vMesh} {}

void GraphicsNode::UseMesh(std::function<void(RenderMesh&)> context) { std::lock_guard<std::mutex> lock(meshMutex); context(mesh); }
void GraphicsNode::UseMeshConst(std::function<void(const RenderMesh&)> context) { std::lock_guard<std::mutex> lock(meshMutex); context(mesh); }
Vector3f GraphicsNode::Position() const { return position; }
GLuint GraphicsNode::VertexBuffer() const { return vertexBuffer; }
GLuint GraphicsNode::UVBuffer() const { return uvBuffer; }

void GraphicsNode::GenerateBuffers() {
    if (!buffersGenerated) {
        buffersGenerated = true;
        glGenBuffers(1, &(GLuint&)vertexBuffer);
        glGenBuffers(1, &(GLuint&)uvBuffer);
    }
}