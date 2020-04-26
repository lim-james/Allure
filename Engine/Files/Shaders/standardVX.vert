#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// instanced
layout(location = 3) in vec4 iTint;
layout(location = 4) in vec3 iPosition;

out VS_OUT {
	vec3 worldPosition;
	vec3 normal;
	vec4 color;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vs_out.worldPosition = inPosition + iPosition;
	vs_out.normal = inNormal;
	vs_out.color = iTint;
	gl_Position = projection * view * vec4(vs_out.worldPosition, 1.f);
}
