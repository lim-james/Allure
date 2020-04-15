#version 330 core

layout(location = 0) in vec3 inPosition;

layout(location = 1) in vec3 iOffset; 
layout(location = 2) in vec3 iLength; 
layout(location = 3) in vec4 iTint;
layout(location = 4) in mat4 iModel;

out VS_OUT {
	vec2 texCoord;
	vec4 color; 
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vec3 position = iOffset + inPosition * iLength;
	gl_Position = projection * view * iModel * vec4(position, 1.f);
	vs_out.texCoord = vec2(0.f);
	vs_out.color = iTint;
}