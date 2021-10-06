#pragma once
#include "GraphicsNode.h"

class ChunkGraphicsNode : public virtual GraphicsNode {
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
    private:
        bool buffersGenerated = false;
        GLuint vertexBuffer;
        GLuint uvBuffer;

        std::mutex meshMutex;
        Mesh mesh;
    public:
        GIVE_TYPE_ID_1(12, GraphicsNode)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunkGraphicsNode)
        explicit ChunkGraphicsNode(const Vector3f& position, GLuint vertexBuffer, GLuint uvBuffer, const Mesh& mesh);
        virtual ~ChunkGraphicsNode() {}

        void UseMesh(std::function<void(Mesh&)> context);
        void UseMeshConst(std::function<void(const Mesh&)> context);

        virtual void Render(const glm::mat4& viewProjection, GLuint matrixId, GLuint textureId, GLuint texture) override;
};