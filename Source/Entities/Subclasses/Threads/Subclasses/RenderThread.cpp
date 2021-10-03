#include "RenderThread.h"
#include "GLUtils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <chrono>
#include <exception>

void RenderThread::AddNode(EntityReference<GraphicsNode> node) {
    std::lock_guard lock(nodesMutex);
    nodes.push_back(node);
}

RenderThread::RenderThread(const std::vector<EntityReference<GraphicsNode>>& vNodes) : Thread(), nodes{vNodes}, clock{std::chrono::system_clock::now()} {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    win = glfwCreateWindow(1280, 720, "Test", NULL, NULL);
    if (win == NULL) {
        glfwTerminate();
        throw std::runtime_error("Failed to open GLFW window");
    }
    glfwMakeContextCurrent(win);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    programId = LoadShaders("res/vertex_shad.glsl", "res/frag_shad.glsl");

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

	matrixId = glGetUniformLocation(programId, "mvp");
	texture = LoadBMP("res/grass.bmp");
	textureId = glGetUniformLocation(programId, "texture_sampler");

    {
        std::lock_guard lock(nodesMutex);
        for (auto& node : nodes) {
            node->GenerateBuffers();
        }
    }
}

void RenderThread::Update() {
    // opengl rendering time lets gooooo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programId);

	float delta = std::chrono::duration<float, std::ratio<1L, 1L>>(std::chrono::high_resolution_clock::now() - clock).count();

	clock = std::chrono::high_resolution_clock::now();
	
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(4, 3, 3),
		glm::vec3(0, 0, 0),
		glm::vec3(sin(0), cos(0), 0)
	);
	glm::mat4 view_proj = proj * view;

	{
        std::lock_guard lock(nodesMutex);
        for (auto& node : nodes) {
            node->GenerateBuffers();
			
			const auto& mesh = node->Mesh();

            glm::mat4 model = glm::translate(glm::mat4(1.0f), node->Position().GLM());
            glm::mat4 mvp = view_proj * model;

			glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1i(textureId, 0);

			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, node->VertexBuffer());
			glBufferData(GL_ARRAY_BUFFER, mesh.triangles.size()*sizeof(float) * 3 * 3, (float*)mesh.triangles.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, node->UVBuffer());
			glBufferData(GL_ARRAY_BUFFER, mesh.triangles.size()*sizeof(float) * 3 * 2, (float*)mesh.uvTriangles.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLES, 0, mesh.triangles.size() * 3);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
        }
    }

	glfwSwapBuffers(win);
	glfwPollEvents();

	// if (glfwWindowShouldClose(win)) {
	// 	scene.destroy();
	// 	break;
	// }
}