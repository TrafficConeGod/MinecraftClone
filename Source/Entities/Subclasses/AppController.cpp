#include "AppController.h"
#include "RenderThread.h"
#include "ChunksThread.h"
#include "UserInterfaceThread.h"

AppController::AppController() : Entity() {
    EntityReference<RenderThread> renderThread = new RenderThread({});
	EntityReference<ChunksThread> chunksThread = new ChunksThread(
		ChunksThread::GraphicsNodeRequested([&](char) {
			chunksThread->RequestedGraphicsNode(renderThread->CreateNode({}));
		})
	);
	EntityReference<UserInterfaceThread> userInterfaceThread = new UserInterfaceThread(
		UserInterfaceThread::CameraUpdated([&](const Vector3f& cameraPosition) {
			chunksThread->CameraUpdate(cameraPosition);
		})
	);

    workers.push_back(renderThread);
    workers.push_back(chunksThread);

    for (auto& worker : workers) {
        worker->Join();
		worker->JoinSubThreads();
    }
}