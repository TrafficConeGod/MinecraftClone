#include "ChunkGraphicsNode.h"
#include "GLUtils.h"

GLuint programId;
GLuint matrixId;
GLuint texture;
GLuint textureId;

void ChunkGraphicsNode::Initialize() {
    programId = LoadShaders("Resources/VertexShader.glsl", "Resources/FragmentShader.glsl");
    matrixId = glGetUniformLocation(programId, "modelViewProjection");
    texture = LoadBMP("Resources/Atlas.bmp");
    textureId = glGetUniformLocation(programId, "textureSampler");
}

ChunkGraphicsNode::Mesh::Mesh() {}
ChunkGraphicsNode::Mesh::Mesh(GLuint vVertexBuffer, GLuint vUvBuffer) : vertexBuffer{vVertexBuffer}, uvBuffer{vUvBuffer} {}

ChunkGraphicsNode::ChunkGraphicsNode(const Vector3f& vPosition, const Mesh& vMesh) : GraphicsNode(vPosition), mesh{vMesh} {}

Mono<ChunkGraphicsNode::Mesh>& ChunkGraphicsNode::ChunkMesh() {
    return mesh;
}

void ChunkGraphicsNode::GenerateBuffersForMeshIfNotGenerated(Mono<Mesh>& mesh) {
    mesh.Use([](auto& mesh) {
        if (!mesh.buffersGenerated) {
            mesh.buffersGenerated = true;
            glGenBuffers(1, &mesh.vertexBuffer);
            glGenBuffers(1, &mesh.uvBuffer);
        }
    });
}

void ChunkGraphicsNode::RenderMesh(const Mono<Mesh>& mesh, const glm::mat4& viewProjection) const {
    mesh.UseConst([&](const auto& mesh) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position.Value().GLM());
        glm::mat4 modelViewProjection = viewProjection * model;

        glUniformMatrix4fv(matrixId, 1, GL_FALSE, &modelViewProjection[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureId, 0);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, mesh.triangles.size()*sizeof(Vector3f) * 3, (float*)mesh.triangles.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, mesh.triangles.size()*sizeof(float) * 3 * 2, (float*)mesh.uvTriangles.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, mesh.triangles.size() * 3);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    });
}

void ChunkGraphicsNode::Render(const glm::mat4& viewProjection) {
    glUseProgram(programId);
    GenerateBuffersForMeshIfNotGenerated(mesh);
    RenderMesh(mesh, viewProjection);
}