#include "AppController.h"
#include "RenderThread.h"
#include "ChunksThread.h"

AppController::AppController() : Entity() {
    EntityReference<RenderThread> renderWorker = new RenderThread({});
    workers.push_back(renderWorker);
    auto chunksNode = renderWorker->CreateNode({
        {
            {
				Vector3f(-1.0f,-1.0f,-1.0f),
				Vector3f(-1.0f,-1.0f, 1.0f),
				Vector3f(-1.0f, 1.0f, 1.0f),
			},
			{
				Vector3f(1.0f, 1.0f,-1.0f),
				Vector3f(-1.0f,-1.0f,-1.0f),
				Vector3f(-1.0f, 1.0f,-1.0f),
			},
			{
				Vector3f(1.0f,-1.0f, 1.0f),
				Vector3f(-1.0f,-1.0f,-1.0f),
				Vector3f(1.0f,-1.0f,-1.0f),
			},
			{
				Vector3f(1.0f, 1.0f,-1.0f),
				Vector3f(1.0f,-1.0f,-1.0f),
				Vector3f(-1.0f,-1.0f,-1.0f),
			},
			{
				Vector3f(-1.0f,-1.0f,-1.0f),
				Vector3f(-1.0f, 1.0f, 1.0f),
				Vector3f(-1.0f, 1.0f,-1.0f),
			},
			{
				Vector3f(1.0f,-1.0f, 1.0f),
				Vector3f(-1.0f,-1.0f, 1.0f),
				Vector3f(-1.0f,-1.0f,-1.0f),
			},
			{
				Vector3f(-1.0f, 1.0f, 1.0f),
				Vector3f(-1.0f,-1.0f, 1.0f),
				Vector3f(1.0f,-1.0f, 1.0f),
			},
			{
				Vector3f(1.0f, 1.0f, 1.0f),
				Vector3f(1.0f,-1.0f,-1.0f),
				Vector3f(1.0f, 1.0f,-1.0f),
			},
			{
				Vector3f(1.0f,-1.0f,-1.0f),
				Vector3f(1.0f, 1.0f, 1.0f),
				Vector3f(1.0f,-1.0f, 1.0f),
			},
			{
				Vector3f(1.0f, 1.0f, 1.0f),
				Vector3f(1.0f, 1.0f,-1.0f),
				Vector3f(-1.0f, 1.0f,-1.0f),
			},
			{
				Vector3f(1.0f, 1.0f, 1.0f),
				Vector3f(-1.0f, 1.0f,-1.0f),
				Vector3f(-1.0f, 1.0f, 1.0f),
			},
			{
				Vector3f(1.0f, 1.0f, 1.0f),
				Vector3f(-1.0f, 1.0f, 1.0f),
				Vector3f(1.0f,-1.0f, 1.0f)
			}
        },
        {
            {
				Vector2f(0.000059f, 1.0f-0.000004f),
				Vector2f(0.000103f, 1.0f-0.336048f),
				Vector2f(0.335973f, 1.0f-0.335903f),
			},
			{
				Vector2f(1.000023f, 1.0f-0.000013f),
				Vector2f(0.667979f, 1.0f-0.335851f),
				Vector2f(0.999958f, 1.0f-0.336064f),
			},
			{
				Vector2f(0.667979f, 1.0f-0.335851f),
				Vector2f(0.336024f, 1.0f-0.671877f),
				Vector2f(0.667969f, 1.0f-0.671889f),
			},
			{
				Vector2f(1.000023f, 1.0f-0.000013f),
				Vector2f(0.668104f, 1.0f-0.000013f),
				Vector2f(0.667979f, 1.0f-0.335851f),
			},
			{
				Vector2f(0.000059f, 1.0f-0.000004f),
				Vector2f(0.335973f, 1.0f-0.335903f),
				Vector2f(0.336098f, 1.0f-0.000071f),
			},
			{
				Vector2f(0.667979f, 1.0f-0.335851f),
				Vector2f(0.335973f, 1.0f-0.335903f),
				Vector2f(0.336024f, 1.0f-0.671877f),
			},
			{
				Vector2f(1.000004f, 1.0f-0.671847f),
				Vector2f(0.999958f, 1.0f-0.336064f),
				Vector2f(0.667979f, 1.0f-0.335851f),
			},
			{
				Vector2f(0.668104f, 1.0f-0.000013f),
				Vector2f(0.335973f, 1.0f-0.335903f),
				Vector2f(0.667979f, 1.0f-0.335851f),
			},
			{
				Vector2f(0.335973f, 1.0f-0.335903f),
				Vector2f(0.668104f, 1.0f-0.000013f),
				Vector2f(0.336098f, 1.0f-0.000071f),
			},
			{
				Vector2f(0.000103f, 1.0f-0.336048f),
				Vector2f(0.000004f, 1.0f-0.671870f),
				Vector2f(0.336024f, 1.0f-0.671877f),
			},
			{
				Vector2f(0.000103f, 1.0f-0.336048f),
				Vector2f(0.336024f, 1.0f-0.671877f),
				Vector2f(0.335973f, 1.0f-0.335903f),
			},
			{
				Vector2f(0.667969f, 1.0f-0.671889f),
				Vector2f(1.000004f, 1.0f-0.671847f),
				Vector2f(0.667979f, 1.0f-0.335851f)
			}
        }
    });
    workers.push_back(new ChunksThread(chunksNode));

    for (auto& worker : workers) {
        worker->Join();
    }
}