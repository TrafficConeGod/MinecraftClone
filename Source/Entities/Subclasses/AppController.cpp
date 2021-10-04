#include "AppController.h"
#include "RenderThread.h"
#include "ChunksThread.h"
#include "UserInterfaceThread.h"

AppController::AppController() : Entity() {
    EntityReference<RenderThread> renderThread = new RenderThread({});
	EntityReference<ChunksThread> chunksThread = new ChunksThread([&]() {
		return renderThread->CreateNode({});
	});
	EntityReference<UserInterfaceThread> userInterfaceThread = new UserInterfaceThread([&](const Vector3f& position) {
		chunksThread->UpdateCamera(position);
	});

    workers.push_back(renderThread);
    workers.push_back(chunksThread);

    for (auto& worker : workers) {
        worker->Join();
		worker->JoinSubThreads();
    }
}