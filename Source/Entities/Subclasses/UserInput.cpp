#include "UserInput.h"

UserInput::UserInput(const KeyCheck& vIsKeyPressed, const KeyCheck& vIsKeyReleased, const KeyCheck& vIsKeyHeld, const CursorCheck& vCursorPosition) : isKeyPressed{vIsKeyPressed}, isKeyReleased{vIsKeyReleased}, isKeyHeld{vIsKeyHeld}, cursorPosition{vCursorPosition} {}

bool UserInput::IsKeyPressed(KeyCode key) const { return isKeyPressed(key); }
bool UserInput::IsKeyReleased(KeyCode key) const { return isKeyReleased(key); }
bool UserInput::IsKeyHeld(KeyCode key) const { return isKeyHeld(key); }
Vector2i UserInput::CursorPosition() const { return cursorPosition(); }