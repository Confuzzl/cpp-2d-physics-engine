#version 460 core

uniform uvec3 frag_color = uvec3(255, 255, 255);

out vec4 color;

void main() {
	color = vec4(frag_color.x / 255.0, frag_color.y / 255.0, frag_color.z / 255.0, 1.0);
}