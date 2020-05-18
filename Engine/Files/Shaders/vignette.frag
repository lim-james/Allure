#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D tex;
uniform float multiplier;
uniform vec3 tint;

void main() {	
	vec2 dist = texCoord - vec2(.5f);
	float r = 1.f - length(dist) * multiplier;

	vec3 result = mix(tint, texture(tex, texCoord).rgb, r);
	color = vec4(result, 1.f);
}