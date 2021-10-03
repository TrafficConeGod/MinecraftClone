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
        std::atomic<bool> buffersGenerated = 0;
        std::atomic<GLuint> vertexBuffer;
        std::atomic<GLuint> uvBuffer;

        std::mutex positionMutex;
        Vector3f position;

        std::mutex meshMutex;
        RenderMesh mesh;
    public:
        GIVE_TYPE_ID_1(5, Entity)
        
        DELETE_ILLEGAL_CONSTRUCTORS(GraphicsNode)
        explicit GraphicsNode(GLuint vertexBuffer, GLuint uvBuffer, const RenderMesh& mesh);
        virtual ~GraphicsNode() {}
        
        void UseMesh(std::function<void(RenderMesh&)> context);
        void UseMeshConst(std::function<void(const RenderMesh&)> context);
        Vector3f Position() const;
        void Position(const Vector3f& position);
        GLuint VertexBuffer() const;
        GLuint UVBuffer() const;

        void GenerateBuffers();
};