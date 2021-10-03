#pragma once
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

GLuint LoadShaders(const char* vertexFilePath, const char* fragmentFilePath);

GLuint LoadBMP(const char* path);

GLuint LoadDDS(const char* path);

void PrintMat4(glm::mat4 mat);