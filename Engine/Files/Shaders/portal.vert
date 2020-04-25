#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in mat4 iModel;

out vec4 position;

uniform mat4 projection;
uniform mat4 view;

void main() {
	position = projection * view * iModel * vec4(inPosition, 1.f);
	gl_Position = position;
}