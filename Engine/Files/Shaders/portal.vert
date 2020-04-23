#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in mat4 iModel;

out vec4 position;
//out vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;

void main() {
	position = projection * view * iModel * vec4(inPosition, 1.f);
//	texCoord = inTexCoord;
	gl_Position = position;
}