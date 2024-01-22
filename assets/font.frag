#version 460 core
in vec2 vertex_uv_out;

out vec4 color;

uniform sampler2D sampler;
uniform vec4 font_color = vec4(1,1,1,1);

void main() {
	vec4 col = texture(sampler, vertex_uv_out) * font_color;
	if (col.a == 0)
		discard;
	color = col;
}