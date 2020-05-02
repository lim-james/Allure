#version 330 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

in vec2 texCoord;

uniform sampler2D fragTex;
uniform sampler2D brightTex;

void main() {	
	fragColor = texture(fragTex, texCoord);
	brightColor = texture(brightTex, texCoord);
}