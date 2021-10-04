#include "GraphicsNode.h"

using Mesh = GraphicsNode::Mesh;

GraphicsNode::GraphicsNode(GLuint vVertexBuffer, GLuint vUvBuffer, const Mesh& vMesh) : Entity(), vertexBuffer{vVertexBuffer}, uvBuffer{vUvBuffer}, mesh{vMesh} {}

void GraphicsNode::UseMesh(std::function<void(Mesh&)> context) { std::lock_guard lock(meshMutex); context(mesh); }
void GraphicsNode::UseMeshConst(std::function<void(const Mesh&)> context) { std::lock_guard lock(meshMutex); context(mesh); }
Vector3f GraphicsNode::Position() const { return position; }
void GraphicsNode::Position(const Vector3f& vPosition) { std::lock_guard lock(positionMutex); position = vPosition; }
GLuint GraphicsNode::VertexBuffer() const { return vertexBuffer; }
GLuint GraphicsNode::UVBuffer() const { return uvBuffer; }

void GraphicsNode::GenerateBuffers() {
    if (!buffersGenerated) {
        buffersGenerated = true;
        glGenBuffers(1, &(GLuint&)vertexBuffer);
        glGenBuffers(1, &(GLuint&)uvBuffer);
    }
}