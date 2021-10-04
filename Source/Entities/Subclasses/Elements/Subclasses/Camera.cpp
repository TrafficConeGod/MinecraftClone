#include "Camera.h"
#include <GLFW/glfw3.h>

static constexpr float Speed = 0.1f;

Camera::Camera(const CoordUpdate& vCoordUpdate, const Vector3f& vPosition, const Vector3f& vLookVector) : coordUpdate{vCoordUpdate} {
    Position(vPosition);
    LookVector(vLookVector);
}

void Camera::Position(const Vector3f& vPosition) {
    position = vPosition;
    coordUpdate(position, lookVector);
}

void Camera::LookVector(const Vector3f& vLookVector) {
    lookVector = vLookVector;
    coordUpdate(position, lookVector);
}

void Camera::Update(const UserInput& userInput) {
    // wasdqe movement
    lookVector = lookVector.Normalize();
    Vector3f inputVector;
    if (userInput.IsKeyHeld(GLFW_KEY_W)) {
        inputVector += Vector3f(lookVector.x, 0, 0);
    }
    if (userInput.IsKeyHeld(GLFW_KEY_S)) {
        inputVector -= Vector3f(lookVector.x, 0, 0);
    }
    if (userInput.IsKeyHeld(GLFW_KEY_E)) {
        inputVector += Vector3f(0, lookVector.y, 0);
    }
    if (userInput.IsKeyHeld(GLFW_KEY_Q)) {
        inputVector -= Vector3f(0, lookVector.y, 0);
    }
    if (userInput.IsKeyHeld(GLFW_KEY_D)) {
        inputVector += Vector3f(0, 0, lookVector.z);
    }
    if (userInput.IsKeyHeld(GLFW_KEY_A)) {
        inputVector -= Vector3f(0, 0, lookVector.z);
    }
    if (inputVector.Magnitude() != 0) {
        // inputVector = inputVector.Normalize();
    }
    std::cout << lookVector << " " << inputVector << "\n";
    Vector3f moveVector = inputVector * Speed;
    Position(position + moveVector);
}