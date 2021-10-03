#include "GraphicsNode.h"

using RenderMesh = GraphicsNode::RenderMesh;

GraphicsNode::GraphicsNode(GLuint vVertexBuffer, GLuint vUvBuffer, const RenderMesh& vMesh) : Entity(), vertexBuffer{vVertexBuffer}, uvBuffer{vUvBuffer}, mesh{vMesh} {}

RenderMesh& GraphicsNode::Mesh() { return mesh; }
const RenderMesh& GraphicsNode::Mesh() const { return mesh; }
const Vector3f& GraphicsNode::Position() const { return position; }
GLuint GraphicsNode::VertexBuffer() const { return vertexBuffer; }
GLuint GraphicsNode::UVBuffer() const { return uvBuffer; }

void GraphicsNode::GenerateBuffers() {
    if (!buffersGenerated) {
        buffersGenerated = true;
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &uvBuffer);
    }
}