#version 330 core

layout(location = 0) in vec3 vertexPositionInModelSpace;
layout(location = 1) in vec2 vertexUv;

out vec2 uv;

uniform mat4 modelViewProjection;

void main() {
    gl_Position = modelViewProjection * vec4(vertexPositionInModelSpace, 1);
    uv = vertexUv;
}