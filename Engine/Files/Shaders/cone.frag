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

uniform float borderWeight;
uniform float range;

void main() {
	vec2 radialUV = (vs_out.texCoord - 0.5f) * 2.f;
	float theta = abs(atan(radialUV.y, radialUV.x));
	if (theta >= range) discard;

	float radius = length(radialUV);

	if (radius > 1.f || (1.f - radius) > borderWeight) 
		discard;
	else 
		radius = 1.f;

	fragColor = radius * vs_out.color;

	if (fragColor.a < 0.01)
		discard;

	vec3 color = fragColor.rgb * fragColor.a;
	float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        brightColor = vec4(color, 1.0);
	} else {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
