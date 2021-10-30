#version 330 core

layout(location = 0) in uint vertexData;

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
    uint vertexId = (vertexData >> 0x18u) & 0xffu;

    float x = float((vertexData >> 0x14u) & 0xfu);
    float y = float((vertexData >> 0x10u) & 0xfu);
    float z = float((vertexData >> 0xcu) & 0xfu);
    
    gl_Position = modelViewProjection * vec4((triangles[vertexId] + vec3(x, y, z)), 1);
    uv = uvTriangles[vertexId];
}