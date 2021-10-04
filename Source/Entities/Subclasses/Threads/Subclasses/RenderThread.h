#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "GraphicsNode.h"
#include <mutex>
#include <chrono>
#include <GLFW/glfw3.h>
#include "UserInputEvents.h"
#include <map>

class RenderThread : public virtual Thread {
    private:
        using KeyCode = UserInputEvents::KeyCode;
    
        std::mutex nodesMutex;
        std::vector<EntityReference<GraphicsNode>> nodes;

        std::mutex keysMutex;
        std::map<KeyCode, bool> pressedKeys;
        std::map<KeyCode, bool> releasedKeys;
        std::map<KeyCode, bool> heldKeys;

        GLFWwindow* win;
        GLuint matrixId;
        GLuint texture;
        GLuint textureId;
        GLuint programId;

        std::mutex bufferIdMutex;
        GLuint currentVertexBufferId = 0;
        GLuint currentUvBufferId = 0;

        std::mutex cameraPositionMutex;
        Vector3f cameraPosition;
    protected:
        virtual void Start() override;
        virtual void Update() override;
    public:
        GIVE_TYPE_ID_1(4, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(RenderThread)
        explicit RenderThread(const std::vector<EntityReference<GraphicsNode>>& nodes);
        virtual ~RenderThread() {}

        void AddNode(EntityReference<GraphicsNode> node);
        EntityReference<GraphicsNode> CreateNode(const GraphicsNode::RenderMesh& mesh);
        EntityReference<GraphicsNode> RemoveNode(std::size_t index);

        void UpdateCamera(const Vector3f& position);

        bool IsKeyPressed(KeyCode key);
        bool IsKeyReleased(KeyCode key);
        bool IsKeyHeld(KeyCode key);
};