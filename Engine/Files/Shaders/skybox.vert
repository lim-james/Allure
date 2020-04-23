#version 330 core

layout(location = 0) in vec3 inPosition;

layout(location = 3) in mat4 iModel;

out vec3 texCoord;

uniform mat4 projection;
uniform mat4 view;

void main() {
	gl_Position = projection * view * iModel * vec4(inPosition, 1.f);
	texCoord = inPosition;
}