#version 330 core
layout (location = 0) in vec2 pos;

uniform float scale;

void main() {
	gl_Position = vec4(scale * pos.x, scale * pos.y, 0.0, 1.0);
}
