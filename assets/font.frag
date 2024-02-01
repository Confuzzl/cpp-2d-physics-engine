#version 460 core
in vec2 vertex_uv_out;

out vec4 color;

uniform sampler2D sampler;
uniform uvec3 font_color = uvec3(255, 255, 255);

void main() {
	const vec4 col = texture(sampler, vertex_uv_out) * vec4(font_color.x / 255.0, font_color.y / 255.0, font_color.z / 255.0, 1.0);
	if (col.a == 0)
		discard;
	color = col;
}