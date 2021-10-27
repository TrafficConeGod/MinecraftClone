#pragma once
#include "Entity.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include <array>
#include <GL/glew.h>
#include <atomic>
#include "Mono.h"
#include <functional>
#include "EntityReference.h"

class GraphicsNode : public virtual Entity {
    public:
        using Remove = std::function<void(EntityReference<GraphicsNode>)>;
    protected:
        Mono<Vector3f> position;
    public:
        GIVE_TYPE_ID_1(5, Entity)
        
        DELETE_ILLEGAL_CONSTRUCTORS(GraphicsNode)
        explicit GraphicsNode(const Vector3f& position);
        virtual ~GraphicsNode() {}
        
        Vector3f Position() const;
        void Position(const Vector3f& position);

        virtual void Render(const glm::mat4& viewProjection) {}
};