#include "AppController.h"
#include "RenderThread.h"
#include "ChunksThread.h"

AppController::AppController() : Entity() {
    EntityReference<GraphicsNode> chunksNode = new GraphicsNode({});

    workers.push_back(new RenderThread({ chunksNode }));
    workers.push_back(new ChunksThread(chunksNode));

    for (auto& worker : workers) {
        worker->Join();
    }
}