#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D tex;

void main() {	
	vec2 dist = texCoord - vec2(.5f);
	float r = length(dist);

	vec2 distortTexCoord = texCoord + dist * sin(r * r * 4.f) * r * .5f;// * r * .5f;
	
	if (distortTexCoord.x < 0 || distortTexCoord.x > 1 ||
		distortTexCoord.y < 0 || distortTexCoord.y > 1) {
		discard;
	}

	vec3 result = texture(tex, distortTexCoord).rgb;
	color = vec4(result, 1.f);
}