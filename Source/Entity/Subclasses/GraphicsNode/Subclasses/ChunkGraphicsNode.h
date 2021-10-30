#pragma once
#include "GraphicsNode.h"

class ChunkGraphicsNode : public virtual GraphicsNode {
    public:
        struct Mesh {
            Mesh();
            Mesh(GLuint vertexBuffer);

            bool buffersGenerated = false;
            GLuint vertexBuffer = 0;

            using Vertex = uint;

            struct Triangle {
                std::array<Vertex, 3> vertices;
            };

            std::vector<Triangle> triangles;
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