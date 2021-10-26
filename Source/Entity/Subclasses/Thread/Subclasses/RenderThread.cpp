#include "RenderThread.h"
#include "GLUtils.h"
#include "ChunkGraphicsNode.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <exception>

void RenderThread::AddNode(EntityReference<GraphicsNode> node) {
    nodes.Use([node](auto& nodes) {
    	nodes.push_back(node);
	});
	currentBufferId++;
}

GLuint RenderThread::BufferId() const { return currentBufferId; }

bool RenderThread::IsKeyPressed(KeyCode key) {
	return pressedKeys.UseForGet<bool>([&](auto& pressedKeys) {
		if (!pressedKeys.count(key)) {
			pressedKeys[key] = false;
			heldKeys.Use([key](auto& heldKeys) {
				heldKeys[key] = false;
			});
		}
		return pressedKeys.at(key);
	});
}

bool RenderThread::IsKeyReleased(KeyCode key) {
	return releasedKeys.UseForGet<bool>([&](auto& releasedKeys) {
		if (!releasedKeys.count(key)) {
			releasedKeys[key] = false;
			heldKeys.Use([key](auto& heldKeys) {
				heldKeys[key] = false;
			});
		}
		return releasedKeys.at(key);
	});
}

bool RenderThread::IsKeyHeld(KeyCode key) {
	return heldKeys.UseForGet<bool>([key](auto& heldKeys) {
		if (!heldKeys.count(key)) {
			heldKeys[key] = false;
		}
		return heldKeys.at(key);
	});
}

Vector2i RenderThread::CursorPosition() const {
	return cursorPosition.Value();
}

void RenderThread::UpdateCamera(const Vector3f& position, const Vector3f& lookVector) {
	cameraPosition.Value(position);
	cameraLookVector.Value(lookVector);
}

RenderThread::RenderThread(const StopApplication& vStopApplication, const std::vector<EntityReference<GraphicsNode>>& vNodes) : stopApplication{vStopApplication}, nodes{vNodes} {}

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
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); 

    glClearColor(44.f/256.f, 157.f/256.f, 222.f/256.f, 0.0f);

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

	ChunkGraphicsNode::Initialize();
}

void RenderThread::Update(float delta) {
    // opengl rendering time lets gooooo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 proj = glm::perspective(glm::radians(70.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	Vector3f currentCameraPosition = cameraPosition.Value();
	Vector3f currentCameraLookVector = cameraLookVector.Value();
	glm::mat4 view = glm::lookAt(
		currentCameraPosition.GLM(),
		(currentCameraPosition + currentCameraLookVector).GLM(),
		glm::vec3(sin(0), cos(0), 0)
	);
	glm::mat4 viewProjection = proj * view;

	nodes.Use([viewProjection](auto& nodes) {
        for (auto& node : nodes) {
			node->Render(viewProjection);
        }
    });

	glfwSwapBuffers(win);
	glfwPollEvents();

	{
		heldKeys.Use([&](auto& heldKeys) {
			for (auto& [key, status] : heldKeys) {
				auto oldStatus = status;
				status = (glfwGetKey(win, key) == GLFW_PRESS) || (glfwGetMouseButton(win, key) == GLFW_PRESS);
				if (status && !oldStatus) {
					pressedKeys.Use([key](auto& pressedKeys) { pressedKeys[key] = true; });
				} else if (status && oldStatus) {
					pressedKeys.Use([key](auto& pressedKeys) { pressedKeys[key] = false; });
				} else if (!status && oldStatus) {
					releasedKeys.Use([key](auto& releasedKeys) { releasedKeys[key] = true; });
				} else if (!status && !oldStatus) {
					releasedKeys.Use([key](auto& releasedKeys) { releasedKeys[key] = false; });
				}
			}
		});
		Vector2<double> position;
		glfwGetCursorPos(win, &position.x, &position.y);
		cursorPosition.Value(position);
	}

	if (glfwWindowShouldClose(win)) {
		stopApplication();
	}
}