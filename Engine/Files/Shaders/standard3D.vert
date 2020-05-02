#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 3) in mat4 iModel;

struct LightSpacePoints {
	vec4 positions[16];
};

out VS_OUT {
	vec3 worldPosition;
	vec3 normal;
	vec2 texCoord;
	LightSpacePoints fragPosLightSpace;
} vs_out;

uniform int lightCount;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrices[16];

uniform vec2 tiling;
uniform vec2 offset;

void main() {
	mat4 modelTransform = iModel * model;
	vec4 worldPosition = modelTransform * vec4(inPosition, 1.f);

	gl_Position = projection * view * worldPosition;

	vs_out.worldPosition = worldPosition.xyz;
	vs_out.normal = mat3(modelTransform) * inNormal;
	vs_out.texCoord = inTexCoord * tiling + offset;

	for (int i = 0; i < lightCount; ++i) {
		vs_out.fragPosLightSpace.positions[i] = lightSpaceMatrices[i] * worldPosition;
	}
}