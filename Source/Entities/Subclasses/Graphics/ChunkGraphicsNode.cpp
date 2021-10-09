#include "ChunkGraphicsNode.h"
#include "GLUtils.h"

ChunkGraphicsNode::Mesh::Vertex ChunkGraphicsNode::Mesh::CreateVertex(u_char x, u_char y, u_char z) {
    // return (x << 0x1c) + (y << 0x18) + (z << 0x14);
    return ChunkGraphicsNode::Mesh::Vertex(x, y, z);
}

ChunkGraphicsNode::Mesh::UVVertex ChunkGraphicsNode::Mesh::CreateUVVertex(u_char x, u_char y) {
    x *= 16;
    y *= 16;
    x += 21; // i need to do this for some reason
    return ChunkGraphicsNode::Mesh::UVVertex(((float)x) / 256.f, 1.f - (((float)y) / 256.f));
}

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

void ChunkGraphicsNode::UseMesh(std::function<void(Mesh&)> context) { std::lock_guard lock(meshMutex); context(mesh); }

void ChunkGraphicsNode::Render(const glm::mat4& viewProjection) {
    glUseProgram(programId);
    // generate buffers
    if (!buffersGenerated) {
        buffersGenerated = true;
        glGenBuffers(1, &(GLuint&)vertexBuffer);
        glGenBuffers(1, &(GLuint&)uvBuffer);
    }

    std::lock_guard lock(positionMutex);
    std::lock_guard lock2(meshMutex);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position.GLM());
    glm::mat4 mvp = viewProjection * model;

    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureId, 0);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    // glDisable(GL_CULL_FACE);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.triangles.size()*sizeof(Mesh::Vertex) * 3, (float*)mesh.triangles.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.triangles.size()*sizeof(float) * 3 * 2, (float*)mesh.uvTriangles.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, mesh.triangles.size() * 3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // glDisable(GL_BLEND);
    // glEnable(GL_CULL_FACE);
}