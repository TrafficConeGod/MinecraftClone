#include "ChunksThread.h"
#include <iostream>

ChunksThread::ChunksThread(EntityReference<GraphicsNode> vNode) : Thread(), node{vNode} {}

void ChunksThread::Update() {
    // handle rendering of chunks
    node->UseMesh([](auto& mesh) {
        auto& pos = mesh.triangles.at(2).vertices.at(2);
        pos = Vector3f(pos.x, pos.y + 0.000001f, pos.z);
    });
}