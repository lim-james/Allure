#version 330 core

layout(location = 0) in vec2 inPosition;
layout(location = 2) in vec4 iRect; // Rect of UV in tilemap
layout(location = 4) in mat4 iModel;

out vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;

void main() {
	gl_Position = projection * view * iModel * vec4(inPosition, 0.f, 1.f);
	texCoord = (inPosition + vec2(0.5)) * iRect.zw + iRect.xy;
}
