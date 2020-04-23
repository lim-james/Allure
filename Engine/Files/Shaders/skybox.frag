#version 330 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

in vec3 texCoord;

uniform samplerCube skybox;

void main() {
	fragColor = texture(skybox, texCoord);
	brightColor = vec4(0);
}