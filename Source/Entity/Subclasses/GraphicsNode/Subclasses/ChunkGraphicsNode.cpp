#include "ChunkGraphicsNode.h"
#include "GLUtils.h"

GLuint programId;
GLuint matrixId;
GLuint texture;
GLuint textureId;

void ChunkGraphicsNode::Initialize() {
    programId = LoadShaders("Resources/VertexShader.glsl", "Resources/FragmentShader.glsl");
    matrixId = glGetUniformLocation(programId, "mvp");
    texture = LoadBMP("Resources/Atlas.bmp");
    textureId = glGetUniformLocation(programId, "textureSampler");
}

ChunkGraphicsNode::ChunkGraphicsNode(const Vector3f& vPosition, GLuint vVertexBuffer, GLuint vUvBuffer, const Mesh& vMesh) : GraphicsNode(vPosition), vertexBuffer{vVertexBuffer}, uvBuffer{vUvBuffer}, mesh{vMesh} {}

SingleUsage<ChunkGraphicsNode::Mesh>& ChunkGraphicsNode::ChunkMesh() {
    return mesh;
}

void ChunkGraphicsNode::Render(const glm::mat4& viewProjection) {
    glUseProgram(programId);
    // generate buffers
    if (!buffersGenerated) {
        buffersGenerated = true;
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &uvBuffer);
    }

    mesh.UseConst([&](auto& mesh) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position.Value().GLM());
        glm::mat4 mvp = viewProjection * model;

        glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureId, 0);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, mesh.triangles.size()*sizeof(Vector3f) * 3, (float*)mesh.triangles.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, mesh.triangles.size()*sizeof(float) * 3 * 2, (float*)mesh.uvTriangles.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, mesh.triangles.size() * 3);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    });
}