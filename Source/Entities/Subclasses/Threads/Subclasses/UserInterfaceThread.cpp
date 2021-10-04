#include "UserInterfaceThread.h"

UserInterfaceThread::UserInterfaceThread(const UserInput& vUserInput, const Camera::CoordUpdate& vUpdateCamera) : userInput{vUserInput}, updateCamera{vUpdateCamera}, camera{new Camera([&](const auto& position, const auto& lookVector) {
    vUpdateCamera(position, lookVector);
}, Vector3f(4, 0, 0), Vector3f(1, -1, 1))} {
    elements.push_back(camera);
}

void UserInterfaceThread::Update() {
    for (auto& element : elements) {
        element->Update(userInput);
    }
}