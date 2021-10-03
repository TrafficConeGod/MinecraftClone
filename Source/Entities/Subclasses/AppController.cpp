#include "AppController.h"
#include "ChunksThread.h"

AppController::AppController() : Entity() {
    workers.push_back(new ChunksThread());

    for (auto& worker : workers) {
        worker->Join();
    }
}