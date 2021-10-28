#version 330 core

layout(location = 0) in vec3 blockPosition;
layout(location = 1) in uint vertexId;

out vec2 uv;

uniform mat4 modelViewProjection;

const vec3 triangles[36] = vec3[36](
    // cube face meshes
        // front face mesh 0
            vec3(1, 1, 1),
            vec3(1, 0, 0),
            vec3(1, 1, 0),
            vec3(1, 1, 1),
            vec3(1, 0, 1),
            vec3(1, 0, 0),
        // back face mesh 2
            vec3(0, 1, 1),
            vec3(0, 1, 0),
            vec3(0, 0, 0),
            vec3(0, 1, 1),
            vec3(0, 0, 0),
            vec3(0, 0, 1),
        // top face mesh 4
            vec3(1, 1, 1),
            vec3(1, 1, 0),
            vec3(0, 1, 0),
            vec3(1, 1, 1),
            vec3(0, 1, 0),
            vec3(0, 1, 1),
        // bottom face mesh 6
            vec3(1, 0, 1),
            vec3(0, 0, 0),
            vec3(1, 0, 0),
            vec3(1, 0, 1),
            vec3(0, 0, 1),
            vec3(0, 0, 0),
        // right face mesh 8
            vec3(1, 1, 1),
            vec3(0, 1, 1),
            vec3(0, 0, 1),
            vec3(1, 1, 1),
            vec3(0, 0, 1),
            vec3(1, 0, 1),
        // left face mesh 10
            vec3(1, 1, 0),
            vec3(0, 0, 0),
            vec3(0, 1, 0),
            vec3(1, 1, 0),
            vec3(1, 0, 0),
            vec3(0, 0, 0)
);

const vec2 uvTriangles[36] = vec2[36](
    // cube face meshes
        // front face mesh 0
            vec2(0, 0),
            vec2(1, 1),
            vec2(1, 0),
            vec2(0, 0),
            vec2(0, 1),
            vec2(1, 1),
        // back face mesh 2
            vec2(1, 0),
            vec2(0, 0),
            vec2(0, 1),
            vec2(1, 0),
            vec2(0, 1),
            vec2(1, 1),
        // top face mesh 4
            vec2(1, 1),
            vec2(1, 0),
            vec2(0, 0),
            vec2(1, 1),
            vec2(0, 0),
            vec2(0, 1),
        // bottom face mesh 6
            vec2(1, 1),
            vec2(0, 0),
            vec2(1, 0),
            vec2(1, 1),
            vec2(0, 1),
            vec2(0, 0),
        // right face mesh 8
            vec2(1, 0),
            vec2(0, 0),
            vec2(0, 1),
            vec2(1, 0),
            vec2(0, 1),
            vec2(1, 1),
        // left face mesh 10
            vec2(0, 0),
            vec2(1, 1),
            vec2(1, 0),
            vec2(0, 0),
            vec2(0, 1),
            vec2(1, 1)
);

void main() {
    gl_Position = modelViewProjection * vec4((triangles[vertexId] + blockPosition), 1);
    uv = uvTriangles[vertexId];
}