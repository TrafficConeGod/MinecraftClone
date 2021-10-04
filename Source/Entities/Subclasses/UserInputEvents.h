#pragma once
#include <functional>

class UserInputEvents {
    public:
        using KeyCode = uint;
        using KeyCheck = std::function<bool(KeyCode)>;
    private:
        KeyCheck isKeyPressed;
        KeyCheck isKeyReleased;
        KeyCheck isKeyHeld;
    public:
        UserInputEvents(const KeyCheck& isKeyPressed, const KeyCheck& isKeyReleased, const KeyCheck& isKeyHeld);

        bool IsKeyPressed(KeyCode key) const;
        bool IsKeyReleased(KeyCode key) const;
        bool IsKeyHeld(KeyCode key) const;
};