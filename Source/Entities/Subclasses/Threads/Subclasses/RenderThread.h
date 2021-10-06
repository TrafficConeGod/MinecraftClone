#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "GraphicsNode.h"
#include <mutex>
#include <chrono>
#include <GLFW/glfw3.h>
#include "UserInput.h"
#include <map>

class RenderThread : public virtual Thread {
    public:
        using StopApplication = std::function<void()>;
    private:
        using KeyCode = UserInput::KeyCode;

        StopApplication stopApplication;
    
        std::mutex nodesMutex;
        std::vector<EntityReference<GraphicsNode>> nodes;

        std::mutex keysMutex;
        std::map<KeyCode, bool> pressedKeys;
        std::map<KeyCode, bool> releasedKeys;
        std::map<KeyCode, bool> heldKeys;
        Vector2i cursorPosition;

        GLFWwindow* win;

        std::mutex bufferIdMutex;
        GLuint currentBufferId = 0;

        std::mutex cameraCoordMutex;
        Vector3f cameraPosition;
        Vector3f cameraLookVector;
    protected:
        virtual void Start() override;
        virtual void Update(float delta) override;
    public:
        GIVE_TYPE_ID_1(4, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(RenderThread)
        explicit RenderThread(const StopApplication& stopApplication, const std::vector<EntityReference<GraphicsNode>>& nodes);
        virtual ~RenderThread() {}

        void AddNode(EntityReference<GraphicsNode> node);
        EntityReference<GraphicsNode> RemoveNode(std::size_t index);

        void UpdateCamera(const Vector3f& position, const Vector3f& lookVector);

        bool IsKeyPressed(KeyCode key);
        bool IsKeyReleased(KeyCode key);
        bool IsKeyHeld(KeyCode key);
        Vector2i CursorPosition();

        GLuint BufferId();
};