#version 330 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

out VS_OUT {
	vec2 texCoord;
	vec4 color;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec4 color;

void main() {
	gl_Position = projection * view * model * vec4(inPosition, 0.f, 1.f);
	vs_out.texCoord = inTexCoord;
	vs_out.color = color;
}
