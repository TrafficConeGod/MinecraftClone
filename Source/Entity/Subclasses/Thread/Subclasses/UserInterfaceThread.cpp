#include "UserInterfaceThread.h"

UserInterfaceThread::UserInterfaceThread(const UserInput& vUserInput, const Camera::CoordUpdate& vUpdateCamera, const Control::MouseClick& vMouseClick) : userInput{vUserInput}, updateCamera{vUpdateCamera}, mouseClick{vMouseClick}, camera{new Camera(std::bind(&UserInterfaceThread::UpdateCamera, this, std::placeholders::_1, std::placeholders::_2), Vector3f(0, 30, 0), Vector3f(1, -1, 1))}, control{new Control(vMouseClick)} {
    elements.push_back(camera);
    elements.push_back(control);
}

void UserInterfaceThread::UpdateCamera(const Vector3f& position, const Vector3f& lookVector) {
    cameraPosition = position;
    cameraLookVector = lookVector;
    updateCamera(position, lookVector);
}

void UserInterfaceThread::Update(float delta) {
    for (auto& element : elements) {
        element->Update({ userInput, delta, cameraPosition, cameraLookVector });
    }
}