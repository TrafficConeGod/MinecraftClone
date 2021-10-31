#pragma once
#include "GraphicsNode.h"

class ChunkGraphicsNode : public virtual GraphicsNode {
    public:
        struct Mesh {
            using Vertex = uint;

            struct Triangle {
                std::array<Vertex, 3> vertices;
            };

            std::vector<Triangle> triangles;
        };

        struct MeshGroup {
            Mesh mainMesh;
        };
    private:
        bool bufferGenerated = false;
        GLuint bufferId = 0;

        Mono<MeshGroup> meshes;

        void GenerateBuffer();
        void RenderMesh(const Mesh& mesh, const glm::mat4& viewProjection) const;
    public:
        static void Initialize();

        GIVE_TYPE_ID_1(12, GraphicsNode)

        DELETE_ILLEGAL_CONSTRUCTORS(ChunkGraphicsNode)
        explicit ChunkGraphicsNode(const Vector3f& position, GLuint bufferId);
        virtual ~ChunkGraphicsNode() {}

        virtual void Render(const glm::mat4& viewProjection) override;

        Mono<MeshGroup>& Meshes();
        const Mono<MeshGroup>& Meshes() const;
};