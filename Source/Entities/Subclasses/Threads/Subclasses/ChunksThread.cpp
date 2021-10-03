#include "ChunksThread.h"
#include <iostream>

ChunksThread::ChunksThread(EntityReference<GraphicsNode> vNode) : Thread(), node{vNode} {}

void ChunksThread::Update() {
    // handle rendering of chunks
    auto& mesh = node->Mesh();
    
}