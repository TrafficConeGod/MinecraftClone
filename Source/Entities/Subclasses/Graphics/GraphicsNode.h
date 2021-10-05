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

        struct Mesh {
            std::vector<Triangle> triangles;
            std::vector<UVTriangle> uvTriangles;
        };
    protected:
        std::atomic<GLuint> vertexBuffer;
        std::atomic<GLuint> uvBuffer;

        std::mutex positionMutex;
        Vector3f position;

        std::mutex meshMutex;
        Mesh mesh;
    public:
        GIVE_TYPE_ID_1(5, Entity)
        
        DELETE_ILLEGAL_CONSTRUCTORS(GraphicsNode)
        explicit GraphicsNode(GLuint vertexBuffer, GLuint uvBuffer, const Mesh& mesh);
        virtual ~GraphicsNode() {}
        
        void UseMesh(std::function<void(Mesh&)> context);
        void UseMeshConst(std::function<void(const Mesh&)> context);
        Vector3f Position() const;
        void Position(const Vector3f& position);

        virtual void Render(const glm::mat4& viewProjection, GLuint matrixId, GLuint textureId, GLuint texture) {}
};