#include "AppController.h"
#include "RenderThread.h"
#include "ChunksThread.h"
#include "UserInterfaceThread.h"
#include "UserInput.h"

AppController::AppController() : Entity() {
    EntityReference<RenderThread> renderThread = new RenderThread({});
	EntityReference<ChunksThread> chunksThread = new ChunksThread([&]() {
		return renderThread->CreateNode({});
	});
	UserInput userInput(
		[&](auto key) { return renderThread->IsKeyPressed(key); },
		[&](auto key) { return renderThread->IsKeyReleased(key); },
		[&](auto key) { return renderThread->IsKeyHeld(key); },
		[&]() { return renderThread->CursorPosition(); }
	);
	EntityReference<UserInterfaceThread> userInterfaceThread = new UserInterfaceThread(userInput, [&](const auto& position, const auto& lookVector) {
		renderThread->UpdateCamera(position, lookVector);
		chunksThread->UpdateCamera(position);
	});

    workers.push_back(renderThread);
    workers.push_back(chunksThread);

    for (auto& worker : workers) {
        worker->Join();
		worker->JoinSubThreads();
    }
}