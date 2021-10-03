#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "GraphicsNode.h"
#include <mutex>
#include <chrono>
#include <GLFW/glfw3.h>

class RenderThread : public virtual Thread {
    private:
        std::mutex nodesMutex;
        std::vector<EntityReference<GraphicsNode>> nodes;

        GLFWwindow* win;
        GLuint matrixId;
        GLuint texture;
        GLuint textureId;
        GLuint programId;
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
};