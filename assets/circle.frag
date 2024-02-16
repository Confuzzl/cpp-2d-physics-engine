#version 460 core

//uniform vec2 center;
//uniform float radius;

uniform uvec3 frag_color = uvec3(255, 255, 255);

out vec4 color;

void main() {
//	const vec2 diff = gl_FragCoord.xy - center;
//	const float d2 = diff.x * diff.x + diff.y + diff.y;
//	if (d2 > radius * radius)
//		discard;
	color = vec4(frag_color.xyz / 255.0, 1.0);
}