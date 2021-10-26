#pragma once
#include <functional>
#include "Vector2.h"

class UserInput {
    public:
        using KeyCode = uint;
        using KeyCheck = std::function<bool(KeyCode)>;
        using CursorCheck = std::function<Vector2i()>;
    private:
        KeyCheck isKeyPressed;
        KeyCheck isKeyReleased;
        KeyCheck isKeyHeld;
        CursorCheck cursorPosition;
    public:
        UserInput(const KeyCheck& isKeyPressed, const KeyCheck& isKeyReleased, const KeyCheck& isKeyHeld, const CursorCheck& cursorPosition);

        bool IsKeyPressed(KeyCode key) const;
        bool IsKeyReleased(KeyCode key) const;
        bool IsKeyHeld(KeyCode key) const;
        Vector2i CursorPosition() const;
};