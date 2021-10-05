#pragma once
#include "GraphicsNode.h"

class ChunkGraphicsNode : public virtual GraphicsNode {
    private:
        std::atomic<bool> buffersGenerated = false;
    public:
        GIVE_TYPE_ID_1(12, GraphicsNode)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunkGraphicsNode)
        using GraphicsNode::GraphicsNode;
        virtual ~ChunkGraphicsNode() {}

        virtual void Render(const glm::mat4& viewProjection, GLuint matrixId, GLuint textureId, GLuint texture) override;
};