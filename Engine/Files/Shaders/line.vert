#version 330 core

layout(location = 0) in vec2 inPosition;

layout(location = 1) in vec2 iOffset; 
layout(location = 2) in vec2 iLength; 
layout(location = 3) in vec4 iTint;

out VS_OUT {
	vec2 texCoord;
	vec4 color; 
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vec2 position = iOffset + inPosition * iLength;
	gl_Position = projection * view * vec4(position, 0.f, 1.f);
	vs_out.texCoord = vec2(0.f);
	vs_out.color = iTint;
}