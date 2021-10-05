#include "RenderThread.h"
#include "GLUtils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <exception>

void RenderThread::AddNode(EntityReference<GraphicsNode> node) {
    std::lock_guard lock(nodesMutex);
    nodes.push_back(node);
}

EntityReference<GraphicsNode> RenderThread::CreateNode(const GraphicsNode::Mesh& mesh) {
	EntityReference<GraphicsNode> node = new GraphicsNode(currentVertexBufferId, currentUvBufferId, mesh);
	AddNode(node);
	std::lock_guard lock(bufferIdMutex);
	currentVertexBufferId++;
	currentUvBufferId++;
	return node;
}

bool RenderThread::IsKeyPressed(KeyCode key) {
	std::lock_guard lock(keysMutex);
	if (!pressedKeys.count(key)) {
		pressedKeys[key] = false;
	}
	return pressedKeys.at(key);
}

bool RenderThread::IsKeyReleased(KeyCode key) {
	std::lock_guard lock(keysMutex);
	if (!releasedKeys.count(key)) {
		releasedKeys[key] = false;
	}
	return releasedKeys.at(key);
}

bool RenderThread::IsKeyHeld(KeyCode key) {
	std::lock_guard lock(keysMutex);
	if (!heldKeys.count(key)) {
		heldKeys[key] = false;
	}
	return heldKeys.at(key);
}

Vector2i RenderThread::CursorPosition() {
	std::lock_guard lock(keysMutex);
	return cursorPosition;
}

void RenderThread::UpdateCamera(const Vector3f& position, const Vector3f& lookVector) {
	std::lock_guard lock(cameraCoordMutex);
	cameraPosition = position;
	cameraLookVector = lookVector;
}

RenderThread::RenderThread(const std::vector<EntityReference<GraphicsNode>>& vNodes) : Thread(), nodes{vNodes} {}

void RenderThread::Start() {
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

    programId = LoadShaders("Resources/VertexShader.glsl", "Resources/FragmentShader.glsl");

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

	matrixId = glGetUniformLocation(programId, "mvp");
	texture = LoadDDS("Resources/Grass.dds");
	textureId = glGetUniformLocation(programId, "texture_sampler");
}

void RenderThread::Update(float delta) {
    // opengl rendering time lets gooooo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programId);
	
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	Vector3f currentCameraPosition;
	Vector3f currentCameraLookVector;
	{
		std::lock_guard lock(cameraCoordMutex);
		currentCameraPosition = cameraPosition;
		currentCameraLookVector = cameraLookVector;
	}
	glm::mat4 view = glm::lookAt(
		currentCameraPosition.GLM(),
		(currentCameraPosition + currentCameraLookVector).GLM(),
		glm::vec3(sin(0), cos(0), 0)
	);
	glm::mat4 viewProjection = proj * view;

	{
        std::lock_guard lock(nodesMutex);
        for (auto& node : nodes) {
			node->Render(viewProjection, matrixId, textureId, texture);
        }
    }

	glfwSwapBuffers(win);
	glfwPollEvents();

	{
		std::lock_guard lock(keysMutex);
		for (auto& [key, status] : heldKeys) {
			auto oldStatus = status;
			status = (glfwGetKey(win, key) == GLFW_PRESS) || (glfwGetMouseButton(win, key) == GLFW_PRESS);
			if (status && !oldStatus) {
				pressedKeys[key] = false;
			} else if (status && oldStatus) {
				pressedKeys[key] = false;
			} else if (!status && oldStatus) {
				releasedKeys[key] = true;
			} else if (!status && !oldStatus) {
				releasedKeys[key] = false;
			}
		}
		Vector2<double> position;
		glfwGetCursorPos(win, &position.x, &position.y);
		cursorPosition = position;
	}

	// if (glfwWindowShouldClose(win)) {
	// 	scene.destroy();
	// 	break;
	// }
}