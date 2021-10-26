#pragma once
#include "GraphicsNode.h"

class ChunkGraphicsNode : public virtual GraphicsNode {
    public:
        struct Mesh {

            struct Triangle {
                std::array<Vector3f, 3> vertices;
            };

            struct UVTriangle {
                std::array<Vector2f, 3> vertices;
            };

            std::vector<Triangle> triangles;
            std::vector<UVTriangle> uvTriangles;
        };
    private:
        bool buffersGenerated = false;
        GLuint vertexBuffer;
        GLuint uvBuffer;

        SingleUsage<Mesh> mesh;
    public:
        static void Initialize();

        GIVE_TYPE_ID_1(12, GraphicsNode)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunkGraphicsNode)
        explicit ChunkGraphicsNode(const Vector3f& position, GLuint vertexBuffer, GLuint uvBuffer, const Mesh& mesh);
        virtual ~ChunkGraphicsNode() {}

        SingleUsage<Mesh>& ChunkMesh();
        const SingleUsage<Mesh>& ChunkMesh() const;

        virtual void Render(const glm::mat4& viewProjection) override;
};