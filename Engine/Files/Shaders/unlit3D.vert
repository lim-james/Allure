#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 3) in mat4 iModel;

out VS_OUT {
	vec3 fragmentPosition;
	vec3 normal;
	vec2 texCoord;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

uniform vec2 tiling;
uniform vec2 offset;

void main() {
	vec4 position = vec4(inPosition, 1.f);

	gl_Position = projection * view * iModel * position;

	vs_out.fragmentPosition = vec3(iModel * position);
	vs_out.normal = mat3(transpose(iModel)) * inNormal;
	vs_out.texCoord = inTexCoord * tiling + offset;
}