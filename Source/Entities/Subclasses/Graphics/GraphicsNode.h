#pragma once
#include "Entity.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include <array>
#include <GL/glew.h>
#include <atomic>
#include <mutex>
#include <functional>

class GraphicsNode : public virtual Entity {
    protected:
        std::mutex positionMutex;
        Vector3f position;
    public:
        GIVE_TYPE_ID_1(5, Entity)
        
        DELETE_ILLEGAL_CONSTRUCTORS(GraphicsNode)
        explicit GraphicsNode(const Vector3f& position);
        virtual ~GraphicsNode() {}
        
        Vector3f Position() const;
        void Position(const Vector3f& position);

        virtual void Render(const glm::mat4& viewProjection) {}
};