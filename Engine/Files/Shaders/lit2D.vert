#version 330 core

layout(location = 0) in vec2 inPosition;

// instanced
layout(location = 2) in vec4 iRect; // Rect of UV in tilemap
layout(location = 3) in vec4 iTint;
layout(location = 4) in mat4 iModel;

out VS_OUT {
	vec3 worldPosition;
	vec2 texCoord;
	vec4 color;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vec4 worldPosition = iModel * vec4(inPosition, 1.f);
	gl_Position = projection * view * worldPosition;
	
	vs_out.worldPosition = worldPosition.xyz;
	vs_out.texCoord = (inPosition + vec2(0.5)) * iRect.zw + iRect.xy;
	vs_out.color = iTint;
}
