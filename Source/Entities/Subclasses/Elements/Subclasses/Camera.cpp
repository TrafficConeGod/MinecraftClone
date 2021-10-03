#include "Camera.h"

Camera::Camera(const Updated& vUpdated) : updated{vUpdated} {
    Position(Vector3f(4, 3, 3));
}

void Camera::Position(const Vector3f& vPosition) {
    position = vPosition;
    updated.Fire(position);
}