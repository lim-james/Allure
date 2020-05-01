#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// instanced
layout(location = 3) in vec4 iTint;
layout(location = 4) in vec3 iPosition;

struct LightSpacePoints {
	vec4 positions[16];
};

out VS_OUT {
	vec3 worldPosition;
	vec3 normal;
	vec4 color;
	LightSpacePoints fragPosLightSpace;
} vs_out;

uniform int lightCount;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 lightSpaceMatrices[16];

void main() {
	vs_out.worldPosition = inPosition + iPosition;
	vs_out.normal = inNormal;
	vs_out.color = iTint;
	gl_Position = projection * view * vec4(vs_out.worldPosition, 1.f);
	
	for (int i = 0; i < lightCount; ++i) {
		vs_out.fragPosLightSpace.positions[i] = lightSpaceMatrices[i] * vec4(vs_out.worldPosition, 1.f);
	}
}
