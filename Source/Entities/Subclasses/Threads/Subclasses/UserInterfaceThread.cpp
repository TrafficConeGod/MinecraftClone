#include "UserInterfaceThread.h"

UserInterfaceThread::UserInterfaceThread(const UserInput& vUserInput, const Camera::CoordUpdate& vUpdateCamera) : userInput{vUserInput}, updateCamera{vUpdateCamera}, camera{new Camera([&](const auto& position) {
    vUpdateCamera(position);
}, Vector3f(4, 0, 0))} {
    elements.push_back(camera);
}

void UserInterfaceThread::Update() {
    for (auto& element : elements) {
        element->Update(userInput);
    }
}