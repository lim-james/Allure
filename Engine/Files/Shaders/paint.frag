#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D tex;
uniform float unit;
uniform float radius;

void main() {
	float inv = 1.f / (radius * radius - 1.f); 
	vec2 texOffset = unit / textureSize(tex, 0); // gets size of single texel

	vec4 color = texture2D(tex, texCoord);

	vec2 m = vec2(0);
	float SobelX[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
	float SobelY[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	vec3 c = vec3(0.3,0.59,0.11);
	int i = 0;

	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			vec2 offset = vec2(x, y) * texOffset;
			vec3 point = texture2D(tex, texCoord + offset).rgb;
			float value = dot(point, c);
        
			m.x += value * SobelX[i];
			m.y += value * SobelY[i];
			++i;
		}
	}

	float angle = atan(m.y, m.x);
	mat2 rotation = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));

	vec3 means[4];
	means[0] = means[1] = means[2] = means[3] = color.rgb;

	vec3 varience[4];
	varience[0] = varience[1] = varience[2] = varience[3] = color.rgb;

	for (float x = 1.f; x < radius; ++x) {
		for (float y = 1.f; y < radius; ++y) {
			vec2 oa = (texOffset * vec2(x, y)) * rotation;
			vec2 ob = (texOffset * vec2(x, -y)) * rotation;
			means[0] += texture2D(tex, texCoord + oa).rgb; // +x, +y
			varience[0] += means[0] * means[0];
			means[1] += texture2D(tex, texCoord - oa).rgb; // -x, -y
			varience[1] += means[1] * means[1];
			means[2] += texture2D(tex, texCoord + ob).rgb; // +x, -y
			varience[2] += means[2] * means[2];
			means[3] += texture2D(tex, texCoord - ob).rgb; // -x, +y	
			varience[3] += means[3] * means[3];
		}
	}

	means[0] *= inv;
	vec3 v = varience[0] * inv - means[0] * means[0];
	float smallest = v.r + v.g + v.b;
	vec3 result = means[0];

	for (int i = 1; i < 4; ++i) {
		means[i] *= inv;
		v = varience[i] * inv - means[i] * means[i];
		float delta = v.r + v.g + v.b;
		if (delta < smallest) {
			smallest = delta;
			result = means[i];
		}
	}

	fragColor = vec4(result.rgb, color.a);
}