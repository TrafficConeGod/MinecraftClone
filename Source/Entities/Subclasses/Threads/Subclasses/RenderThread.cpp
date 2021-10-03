#include "RenderThread.h"

RenderThread::RenderThread(const std::vector<EntityReference<GraphicsNode>>& vNodes) : Thread(), nodes{vNodes} {}

void RenderThread::Update() {}