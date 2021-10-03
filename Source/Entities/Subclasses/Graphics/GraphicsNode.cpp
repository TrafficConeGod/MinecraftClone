#include "GraphicsNode.h"

using Triangle = GraphicsNode::Triangle;

GraphicsNode::GraphicsNode(const std::vector<Triangle>& vMesh) : Entity(), mesh{vMesh} {}

std::vector<Triangle>& GraphicsNode::Mesh() { return mesh; }
const std::vector<Triangle>& GraphicsNode::Mesh() const { return mesh; }