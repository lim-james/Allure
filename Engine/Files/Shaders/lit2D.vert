#version 330 core

layout(location = 0) in vec2 inPosition;

// instanced
layout(location = 2) in vec4 iRect; // Rect of UV in tilemap
layout(location = 3) in vec4 iTint;
layout(location = 4) in mat4 iModel;

out VS_OUT {
	vec3 worldPosition;
	vec3 normal; 
	vec2 texCoord;
	vec2 screenUV;
	vec2 lightUVs[8];
	vec4 color;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

uniform int lightCount;
uniform vec3 lightPositions[8];

void main() {
	vec4 position = iModel * vec4(inPosition, 0.f, 1.f);
	mat4 vp = projection * view;
	gl_Position = vp * position;
	
	vs_out.worldPosition = position.xyz;
	vs_out.normal = vs_out.worldPosition - (iModel * vec4(0.f, 0.f, 0.f, 1.f)).xyz;
	vs_out.texCoord = (inPosition + vec2(0.5)) * iRect.zw + iRect.xy;
	vs_out.screenUV = ((gl_Position.xy / gl_Position.w) + 1.f) * 0.5f;

    for (int i = 0; i < lightCount; ++i) {
		vec4 lightPosition = vp * vec4(lightPositions[i], 1.f);
		vs_out.lightUVs[i] = ((lightPosition.xy / lightPosition.w) + 1.f) * 0.5f;
	}

	vs_out.color = iTint;
}
