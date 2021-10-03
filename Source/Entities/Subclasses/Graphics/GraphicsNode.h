#pragma once
#include "Entity.h"
#include "Vector3.h"
#include <vector>
#include <array>

class GraphicsNode : public virtual Entity {
    public:
        struct Triangle {
            std::array<Vector3f, 3> vertices;
        };
    private:
        std::vector<Triangle> mesh;
    public:
        GIVE_TYPE_ID_1(5, Entity)
        
        DELETE_ILLEGAL_CONSTRUCTORS(GraphicsNode)
        explicit GraphicsNode(const std::vector<Triangle>& mesh);
        virtual ~GraphicsNode() {}

        std::vector<Triangle>& Mesh();
        const std::vector<Triangle>& Mesh() const;
};