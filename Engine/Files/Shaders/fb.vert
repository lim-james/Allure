#version 330 core

layout(location = 0) in vec2 inPosition;

out vec2 texCoord;

void main() {
	gl_Position = vec4(inPosition * 2.f, 0.f, 1.f);
	texCoord = inPosition + vec2(0.5);
}
