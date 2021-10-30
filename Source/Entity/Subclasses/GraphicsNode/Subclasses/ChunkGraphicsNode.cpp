#include "ChunkGraphicsNode.h"
#include "GLUtils.h"

GLuint programId;
GLuint matrixId;
GLuint texture;
GLuint textureId;

void ChunkGraphicsNode::Initialize() {
    programId = LoadShaders("Resources/Shaders/ChunkVertexShader.glsl", "Resources/Shaders/ChunkFragmentShader.glsl");
    matrixId = glGetUniformLocation(programId, "modelViewProjection");
    texture = LoadBMP("Resources/Textures/Chunk.bmp");
    textureId = glGetUniformLocation(programId, "textureSampler");
}

ChunkGraphicsNode::Mesh::Mesh() {}
ChunkGraphicsNode::Mesh::Mesh(GLuint vVertexBuffer) : vertexBuffer{vVertexBuffer} {}

ChunkGraphicsNode::ChunkGraphicsNode(const Vector3f& vPosition, const Mesh& vMesh) : GraphicsNode(vPosition), mainMesh{vMesh} {}

Mono<ChunkGraphicsNode::Mesh>& ChunkGraphicsNode::MainMesh() {
    return mainMesh;
}

void ChunkGraphicsNode::GenerateBuffersForMeshIfNotGenerated(Mono<Mesh>& mesh) {
    mesh.Use([](auto& mesh) {
        if (!mesh.buffersGenerated) {
            mesh.buffersGenerated = true;
            glGenBuffers(1, &mesh.vertexBuffer);
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
        glBufferData(GL_ARRAY_BUFFER, mesh.triangles.size()*sizeof(Mesh::Vertex) * 3, (uint*)mesh.triangles.data(), GL_STATIC_DRAW);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, mesh.triangles.size() * 3);

        glDisableVertexAttribArray(0);
    });
}

void ChunkGraphicsNode::Render(const glm::mat4& viewProjection) {
    glUseProgram(programId);
    GenerateBuffersForMeshIfNotGenerated(mainMesh);
    RenderMesh(mainMesh, viewProjection);
}