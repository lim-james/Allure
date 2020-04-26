#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float exposure;

void main() {
	float gamma = 2.2;

	vec3 standard = texture(tex0, texCoord).rgb;
	vec3 bright = texture(tex1, texCoord).rgb;

	standard += bright;

	vec3 result = vec3(1.0) - exp(-standard * exposure);
	result = pow(result, vec3(1.0 / gamma));
	color = vec4(result, 1.0);
}
