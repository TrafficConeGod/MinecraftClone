#include "Camera.h"
#include <GLFW/glfw3.h>

static constexpr float Speed = 10.f;
static constexpr float RotateSpeed = 7.f;
static constexpr int WindowSizeX = 1280;
static constexpr int WindowSizeY = 720;

Camera::Camera(const CoordUpdate& vCoordUpdate, const Vector3f& vPosition, const Vector3f& vLookVector) : coordUpdate{vCoordUpdate} {
    Position(vPosition);
    LookVector(vLookVector);
}

void Camera::Position(const Vector3f& vPosition) {
    position = vPosition;
    coordUpdate(position, lookVector);
}

void Camera::LookVector(const Vector3f& vLookVector) {
    lookVector = vLookVector.Unit();
    coordUpdate(position, lookVector);
}

void Camera::Update(const UpdateInfo& updateInfo) {
    const auto& userInput = updateInfo.userInput;
    Vector2i cursorPosition = userInput.CursorPosition();
    if (cursorPosition.x >= 0 && cursorPosition.y >= 0 && cursorPosition.x < WindowSizeX && cursorPosition.y < WindowSizeY) {
        Vector2f inputVector = lastCursorPosition - cursorPosition;
        inputVector /= Vector2f(WindowSizeX, WindowSizeY);

        if (lookVector.Magnitude() > 0) {
            Vector2f moveVector = inputVector * RotateSpeed;

            // https://stackoverflow.com/questions/2782647/how-to-get-yaw-pitch-and-roll-from-a-3d-vector
            // https://stackoverflow.com/questions/10569659/camera-pitch-yaw-to-direction-vector

            double pitch = -(std::asin(-lookVector.y));
            double yaw = M_PI - std::atan2(lookVector.x, lookVector.z);

            pitch -= moveVector.y;
            yaw += moveVector.x;

            double xzLength = std::cos(pitch);
            Vector3f newLookVector(xzLength * std::cos(yaw), std::sin(pitch), xzLength * std::sin(-yaw));

            LookVector(newLookVector);
        }
    }
    lastCursorPosition = cursorPosition;

    // wasdqe movement
    Vector3f inputVector;
    if (userInput.IsKeyHeld(GLFW_KEY_D)) {
        inputVector += Vector3f(1, 0, 0);
    }
    if (userInput.IsKeyHeld(GLFW_KEY_A)) {
        inputVector -= Vector3f(1, 0, 0);
    }
    if (userInput.IsKeyHeld(GLFW_KEY_E)) {
        inputVector += Vector3f(0, 1, 0);
    }
    if (userInput.IsKeyHeld(GLFW_KEY_Q)) {
        inputVector -= Vector3f(0, 1, 0);
    }
    if (userInput.IsKeyHeld(GLFW_KEY_W)) {
        inputVector += Vector3f(0, 0, 1);
    }
    if (userInput.IsKeyHeld(GLFW_KEY_S)) {
        inputVector -= Vector3f(0, 0, 1);
    }
    if (inputVector.Magnitude() != 0) {
        inputVector = inputVector.Unit();

        Vector3f regularLookVector = lookVector;
        Vector3f perpendicularLookVector(-lookVector.z, 0, lookVector.x);
        Vector3f verticalLookVector(0, -regularLookVector.y, 0);

        Vector3f moveVector = (lookVector * inputVector.z) + (perpendicularLookVector * inputVector.x) + (verticalLookVector * inputVector.y);
        moveVector = moveVector.Unit();
        moveVector *= Speed * updateInfo.delta;
        Position(position + moveVector);
    }
}