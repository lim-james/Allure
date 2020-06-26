#version 330 core
#define M_PI 3.1415926535897932384626433832795

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

in VS_OUT {
	vec2 texCoord;
	vec4 color;
} vs_out;

uniform bool useTex;
uniform sampler2D tex;

const int size = 491;

uniform float minRadius;
uniform float maxValue;
uniform float unit;
uniform float multiplier;

uniform float outlineWeight;
uniform vec3 outlineColor;

uniform vec2 values[size];

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
	vec2 radialUV = (vs_out.texCoord - 0.5f) * 2.f;
	float radius = length(radialUV);
	float value = max(radius - minRadius + 0.5f, 0.f) / (1.001f - minRadius);	
//	float value = radius + 0.5f - minRadius;
//	float value = floor(1.f - max(radius - minRadius, 0.f) / (1.f - minRadius));	

	vec2 dir = normalize(radialUV);
	float r = acos(dir.y) / M_PI;

	int index = int(r * unit * size);
	vec2 freq = values[index] * multiplier;

//	float delta = M_PI / float(size / 2);
//	vec2 freq = vec2(0.f);

//	float stride = 3.f / float(size);
//	float sampleSize = 5.f;
//	for (float i = -sampleSize; i <= sampleSize; ++i) {
//		float index = r + i * stride;
//		freq += values[clamp(int(index * unit * size), 0, size)];
//	}
//
//	freq *= multiplier / (sampleSize * 2.f + 1.f);

	vec2 UV = vs_out.texCoord - vec2(minRadius * 0.55f);
	UV /= minRadius * 0.5f;
	vec4 texColor = texture(tex, UV) * vs_out.color;

	float a = clamp(round(1.f - value), 0, 1);
	float b = min(1.f - round(value - freq.y), 1.f);
	float c = min(1.f - round(value - freq.y - outlineWeight * radius * radius), 1.f);

	vec3 ball =  a * texColor.rgb;
	vec3 bars =  (b - a) * vs_out.color.rgb;
	vec3 outline = (c - b) * outlineColor;
//	vec3 outline = (b - a) * hsv2rgb(vec3(radius, 1, 1));

	fragColor = vec4(ball + bars + outline, vs_out.color.a);

	if (a + b + c < 0.01f)  
		discard;
		
	if (a < 0.001f)
		brightColor = vec4(fragColor.rgb, 1.0);
	else
		brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
