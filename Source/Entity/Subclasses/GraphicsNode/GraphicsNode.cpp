#include "GraphicsNode.h"

GraphicsNode::GraphicsNode(const Vector3f& vPosition) : position{vPosition} {}

Vector3f GraphicsNode::Position() const { return position.Value(); }
void GraphicsNode::Position(const Vector3f& vPosition) { position.Value(vPosition); }