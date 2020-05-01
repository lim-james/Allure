#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 3) in mat4 iModel;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	vec4 position = vec4(inPosition, 1.f);
	gl_Position = projection * view * iModel * model  * position;
}