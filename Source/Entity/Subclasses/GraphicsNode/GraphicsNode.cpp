#include "GraphicsNode.h"

GraphicsNode::GraphicsNode(const Vector3f& vPosition) : position{vPosition} {}

Vector3f GraphicsNode::Position() const { return position; }
void GraphicsNode::Position(const Vector3f& vPosition) { std::lock_guard lock(positionMutex); position = vPosition; }