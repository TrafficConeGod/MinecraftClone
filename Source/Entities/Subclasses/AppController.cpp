#include "AppController.h"
#include "RenderThread.h"
#include "ChunksThread.h"
#include "UserInterfaceThread.h"
#include "UserInputEvents.h"

AppController::AppController() : Entity() {
    EntityReference<RenderThread> renderThread = new RenderThread({});
	EntityReference<ChunksThread> chunksThread = new ChunksThread([&]() {
		return renderThread->CreateNode({});
	});
	UserInputEvents userInputEvents(
		[&](auto key) { return renderThread->IsKeyPressed(key); },
		[&](auto key) { return renderThread->IsKeyReleased(key); },
		[&](auto key) { return renderThread->IsKeyHeld(key); }
	);
	EntityReference<UserInterfaceThread> userInterfaceThread = new UserInterfaceThread(userInputEvents, [&](const auto& position) {
		chunksThread->UpdateCamera(position);
	});

    workers.push_back(renderThread);
    workers.push_back(chunksThread);

    for (auto& worker : workers) {
        worker->Join();
		worker->JoinSubThreads();
    }
}