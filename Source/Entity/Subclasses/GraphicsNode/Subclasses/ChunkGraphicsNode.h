#pragma once
#include "GraphicsNode.h"

class ChunkGraphicsNode : public virtual GraphicsNode {
    public:
        struct Mesh {
            Mesh();
            Mesh(GLuint positionBuffer, GLuint vertexIdBuffer);

            bool buffersGenerated = false;
            GLuint positionBuffer = 0;
            GLuint vertexIdBuffer = 0;

            struct PositionTriangle {
                std::array<Vector3f, 3> blockPositions;
            };

            struct VertexIdTriangle {
                std::array<uint, 3> vertexIds;
            };

            std::vector<PositionTriangle> positionTriangles;
            std::vector<VertexIdTriangle> vertexIdTriangles;
        };
    private:
        Mono<Mesh> mainMesh;

        void GenerateBuffersForMeshIfNotGenerated(Mono<Mesh>& mesh);
        void RenderMesh(const Mono<Mesh>& mesh, const glm::mat4& viewProjection) const;
    public:
        static void Initialize();

        GIVE_TYPE_ID_1(12, GraphicsNode)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunkGraphicsNode)
        explicit ChunkGraphicsNode(const Vector3f& position, const Mesh& mesh);
        virtual ~ChunkGraphicsNode() {}

        Mono<Mesh>& MainMesh();
        const Mono<Mesh>& MainMesh() const;

        virtual void Render(const glm::mat4& viewProjection) override;
};