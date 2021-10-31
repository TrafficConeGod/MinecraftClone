#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D textureSampler;

void main() {
	color = texture(textureSampler, vec2((uv.x + 1.3125) * 0.0625, 1 - (uv.y * 0.0625))).rgba;
}