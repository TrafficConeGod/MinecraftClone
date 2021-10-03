#pragma once
#include "Entity.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include <array>
#include <GL/glew.h>

class GraphicsNode : public virtual Entity {
    public:
        struct Triangle {
            std::array<Vector3f, 3> vertices;
        };

        struct UVTriangle {
            std::array<Vector2f, 3> vertices;
        };

        struct RenderMesh {
            std::vector<Triangle> triangles;
            std::vector<UVTriangle> uvTriangles;
        };
    private:
        bool buffersGenerated = 0;
        GLuint vertexBuffer;
        GLuint uvBuffer;

        Vector3f position;
        RenderMesh mesh;
    public:
        GIVE_TYPE_ID_1(5, Entity)
        
        DELETE_ILLEGAL_CONSTRUCTORS(GraphicsNode)
        explicit GraphicsNode(GLuint vertexBuffer, GLuint uvBuffer, const RenderMesh& mesh);
        virtual ~GraphicsNode() {}
        
        RenderMesh& Mesh();
        const RenderMesh& Mesh() const;
        const Vector3f& Position() const;
        GLuint VertexBuffer() const;
        GLuint UVBuffer() const;

        void GenerateBuffers();
};