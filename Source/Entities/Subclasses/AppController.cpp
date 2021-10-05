#include "AppController.h"
#include "RenderThread.h"
#include "ChunksThread.h"
#include "UserInterfaceThread.h"
#include "UserInput.h"
#include "ChunkGraphicsNode.h"

AppController::AppController() : Entity() {
    EntityReference<RenderThread> renderThread = new RenderThread([&]() {
		for (auto& worker : workers) {
			worker->RequestStop();
		}
	}, {});
	EntityReference<ChunksThread> chunksThread = new ChunksThread([&]() {
		return renderThread->AddNode(new ChunkGraphicsNode(renderThread->VertexBufferId(), renderThread->UVBufferId(), {}));
	});
	UserInput userInput(
		std::bind(&RenderThread::IsKeyPressed, (RenderThread*)renderThread, std::placeholders::_1),
		std::bind(&RenderThread::IsKeyReleased, (RenderThread*)renderThread, std::placeholders::_1),
		std::bind(&RenderThread::IsKeyHeld, (RenderThread*)renderThread, std::placeholders::_1),
		std::bind(&RenderThread::CursorPosition, (RenderThread*)renderThread)
	);
	EntityReference<UserInterfaceThread> userInterfaceThread = new UserInterfaceThread(userInput, [&](const auto& position, const auto& lookVector) {
		renderThread->UpdateCamera(position, lookVector);
		chunksThread->UpdateCamera(position);
	});

    workers.push_back(renderThread);
    workers.push_back(chunksThread);
    workers.push_back(userInterfaceThread);

    for (auto& worker : workers) {
        worker->Join();
		worker->JoinSubThreads();
    }
}