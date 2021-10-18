#include "Control.h"
#include <GLFW/glfw3.h>

Control::Control(const MouseClick& vMouseClick) : mouseClick{vMouseClick} {}

void Control::Update(const UpdateInfo& updateInfo) {
    if (updateInfo.userInput.IsKeyPressed(GLFW_MOUSE_BUTTON_1)) {
        mouseClick(updateInfo.cameraPosition, updateInfo.cameraLookVector);
    }
}