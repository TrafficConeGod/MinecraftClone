#include "UserInterfaceThread.h"

UserInterfaceThread::UserInterfaceThread(CameraUpdated vCameraUpdated) : cameraUpdated{vCameraUpdated}, camera{new Camera(
    Camera::Updated([&](const Vector3f& position) {
        vCameraUpdated.Fire(position);
    })
)} {
    elements.push_back(camera);
}

void UserInterfaceThread::Update() {
    for (auto& element : elements) {
        element->Update();
    }
}