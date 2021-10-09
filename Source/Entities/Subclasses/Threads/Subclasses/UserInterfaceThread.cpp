#include "UserInterfaceThread.h"

UserInterfaceThread::UserInterfaceThread(const UserInput& vUserInput, const Camera::CoordUpdate& vUpdateCamera) : userInput{vUserInput}, updateCamera{vUpdateCamera}, camera{new Camera(vUpdateCamera, Vector3f(4, 0, 0), Vector3f(-1, 0, 0))} {
    elements.push_back(camera);
}

void UserInterfaceThread::Update(float delta) {
    for (auto& element : elements) {
        element->Update(userInput, delta);
    }
}