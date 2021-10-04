#include "UserInterfaceThread.h"

UserInterfaceThread::UserInterfaceThread(const Camera::Update& vUpdateCamera) : updateCamera{vUpdateCamera}, camera{new Camera([&](const Vector3f& position) {
    vUpdateCamera(position);
})} {
    elements.push_back(camera);
}

void UserInterfaceThread::Update() {
    for (auto& element : elements) {
        element->Update();
    }
}