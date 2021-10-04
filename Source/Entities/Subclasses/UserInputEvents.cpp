#include "UserInputEvents.h"

UserInputEvents::UserInputEvents(const KeyCheck& vIsKeyPressed, const KeyCheck& vIsKeyReleased, const KeyCheck& vIsKeyHeld) : isKeyPressed{vIsKeyPressed}, isKeyReleased{vIsKeyReleased}, isKeyHeld{vIsKeyHeld} {}

bool UserInputEvents::IsKeyPressed(KeyCode key) const { return isKeyPressed(key); }
bool UserInputEvents::IsKeyReleased(KeyCode key) const { return isKeyReleased(key); }
bool UserInputEvents::IsKeyHeld(KeyCode key) const { return isKeyHeld(key); }