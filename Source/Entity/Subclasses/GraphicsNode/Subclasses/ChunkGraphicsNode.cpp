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

ChunkGraphicsNode::ChunkGraphicsNode(const Vector3f& vPosition, GLuint vBufferId) : GraphicsNode(vPosition), bufferId{vBufferId} {}

Mono<ChunkGraphicsNode::MeshGroup>& ChunkGraphicsNode::Meshes() {
    return meshes;
}

const Mono<ChunkGraphicsNode::MeshGroup>& ChunkGraphicsNode::Meshes() const {
    return meshes;
}

void ChunkGraphicsNode::RenderMesh(const Mesh& mesh, const glm::mat4& viewProjection) const {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position.Value().GLM());
    glm::mat4 modelViewProjection = viewProjection * model;

    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &modelViewProjection[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureId, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, mesh.triangles.size()*sizeof(Mesh::Vertex) * 3, (uint*)mesh.triangles.data(), GL_STATIC_DRAW);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, mesh.triangles.size() * 3);

    glDisableVertexAttribArray(0);
}

void ChunkGraphicsNode::Render(const glm::mat4& viewProjection) {
    glUseProgram(programId);
    if (!bufferGenerated) {
        bufferGenerated = true;
        glGenBuffers(1, &bufferId);
    }
    meshes.UseConst([&](const auto& meshes) {
        RenderMesh(meshes.mainMesh, viewProjection);
    });
}