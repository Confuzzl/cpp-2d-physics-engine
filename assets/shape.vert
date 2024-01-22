#version 460 core
layout (location = 0) in vec2 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1.0, 1.0);
}

