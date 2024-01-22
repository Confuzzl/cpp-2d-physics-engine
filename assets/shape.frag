#version 460 core
uniform vec4 frag_color = vec4(1,1,1,1);

out vec4 color;

void main() {
	color = frag_color;
}