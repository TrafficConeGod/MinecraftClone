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
		EntityReference<ChunkGraphicsNode> node = new ChunkGraphicsNode(Vector3f(0, 0, 0), renderThread->BufferId(), renderThread->BufferId(), {});
		renderThread->AddNode(node);
		return node;
	}, std::bind(&RenderThread::RemoveNode, (RenderThread*)renderThread, std::placeholders::_1), 100);
	UserInput userInput(
		std::bind(&RenderThread::IsKeyPressed, (RenderThread*)renderThread, std::placeholders::_1),
		std::bind(&RenderThread::IsKeyReleased, (RenderThread*)renderThread, std::placeholders::_1),
		std::bind(&RenderThread::IsKeyHeld, (RenderThread*)renderThread, std::placeholders::_1),
		std::bind(&RenderThread::CursorPosition, (RenderThread*)renderThread)
	);
	EntityReference<UserInterfaceThread> userInterfaceThread = new UserInterfaceThread(userInput, [&](auto& position, auto& lookVector) {
		renderThread->UpdateCamera(position, lookVector);
		chunksThread->UpdateCamera(position);
	}, [&]( auto& position, auto& lookVector) {
		// TODO: Use actors here
		chunksThread->SignalMouseClick(position, lookVector);
	});

    workers.push_back(renderThread);
    workers.push_back(chunksThread);
    workers.push_back(userInterfaceThread);

    for (auto& worker : workers) {
        worker->Join();
		worker->JoinSubThreads();
    }
}