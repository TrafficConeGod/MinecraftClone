#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "GraphicsNode.h"
#include "Mono.h"
#include <atomic>
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
    
        Mono<std::vector<EntityReference<GraphicsNode>>> nodes;

        Mono<std::map<KeyCode, bool>> pressedKeys;
        Mono<std::map<KeyCode, bool>> releasedKeys;
        Mono<std::map<KeyCode, bool>> heldKeys;
        Mono<Vector2i> cursorPosition;

        GLFWwindow* win;

        std::atomic<GLuint> currentBufferId = 0;

        Mono<Vector3f> cameraPosition;
        Mono<Vector3f> cameraLookVector;
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
        Vector2i CursorPosition() const;
        GLuint BufferId() const;
};