#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 4) in vec3 iPosition;

uniform mat4 projection;
uniform mat4 view;

void main() {
	gl_Position = projection * view * vec4(inPosition + iPosition, 1.f);
}
