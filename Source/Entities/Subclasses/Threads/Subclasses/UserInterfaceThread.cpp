#include "UserInterfaceThread.h"

UserInterfaceThread::UserInterfaceThread(const UserInputEvents& vUserInputEvents, const Camera::CoordUpdate& vUpdateCamera) : userInputEvents{vUserInputEvents}, updateCamera{vUpdateCamera}, camera{new Camera([&](const auto& position) {
    vUpdateCamera(position);
})} {
    elements.push_back(camera);
}

void UserInterfaceThread::Update() {
    for (auto& element : elements) {
        element->Update(userInputEvents);
    }
}