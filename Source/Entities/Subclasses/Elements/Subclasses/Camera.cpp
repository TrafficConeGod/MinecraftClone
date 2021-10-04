#include "Camera.h"

Camera::Camera(const Update& vUpdate) : update{vUpdate} {
    Position(Vector3f(4, 3, 3));
}

void Camera::Position(const Vector3f& vPosition) {
    position = vPosition;
    update(position);
}