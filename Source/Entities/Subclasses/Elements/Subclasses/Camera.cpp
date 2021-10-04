#include "Camera.h"
#include <GLFW/glfw3.h>

static constexpr float Speed = 0.1f;

Camera::Camera(const CoordUpdate& vCoordUpdate, const Vector3f& vPosition) : coordUpdate{vCoordUpdate} {
    Position(vPosition);
}

void Camera::Position(const Vector3f& vPosition) {
    position = vPosition;
    coordUpdate(position);
}

void Camera::Update(const UserInputEvents& userInputEvents) {
    // wasdqe movement
    Vector3f inputVector;
    if (userInputEvents.IsKeyHeld(GLFW_KEY_W)) {
        inputVector += Vector3f(1, 0, 0);
    }
    if (userInputEvents.IsKeyHeld(GLFW_KEY_S)) {
        inputVector -= Vector3f(1, 0, 0);
    }
    if (userInputEvents.IsKeyHeld(GLFW_KEY_E)) {
        inputVector += Vector3f(0, 1, 0);
    }
    if (userInputEvents.IsKeyHeld(GLFW_KEY_Q)) {
        inputVector -= Vector3f(0, 1, 0);
    }
    if (userInputEvents.IsKeyHeld(GLFW_KEY_A)) {
        inputVector += Vector3f(0, 0, 1);
    }
    if (userInputEvents.IsKeyHeld(GLFW_KEY_D)) {
        inputVector -= Vector3f(0, 0, 1);
    }
    Vector3f moveVector = inputVector * Speed;
    Position(position + moveVector);
}