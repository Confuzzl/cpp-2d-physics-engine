#version 460 core

uniform vec2 center;
uniform float radius = 1;

uniform vec2 quad_vertices[4] = vec2[4](
	vec2(-1.0, -1.0), vec2(+1.0, -1.0),
	vec2(+1.0, +1.0), vec2(-1.0, +1.0)
);


void main() {
//	gl_Position = vec4(center + quad_vertices[gl_VertexID] * radius, 0.0, 1.0);
	gl_Position = vec4(quad_vertices[gl_VertexID], 0.0, 1.0);
}

