#include "Camera.h"

Camera::Camera(const CoordUpdate& vCoordUpdate) : coordUpdate{vCoordUpdate} {
    Position(Vector3f(4, 3, 3));
}

void Camera::Position(const Vector3f& vPosition) {
    position = vPosition;
    coordUpdate(position);
}

void Camera::Update(const UserInputEvents& userInputEvents) {
}