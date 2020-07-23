#version 330 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

in VS_OUT {
	vec4 worldPosition;
	vec2 texCoord;
	vec4 color;
} vs_out;

uniform float et;
uniform float speed;

struct Spread {
	float et;
	vec3 tint;
};

uniform float spreadWeight;
uniform float spreadAlpha;
uniform int spreadCount;
uniform Spread spreads[4];

uniform float indicatorWeight;
uniform float indicatorRadius;
uniform float indicatorAlpha;
uniform vec3 indicatorTint;
uniform float viewSize;
uniform float interval;
uniform float bt;

uniform float thresholdWeight;
uniform float thresholdRadius;
uniform float thresholdAlpha;
uniform vec3 thresholdTint;
uniform float threshold;

uniform vec3 playerPosition;

uniform bool useTex;
uniform sampler2D tex;

const float PI = 3.14159265359;

//void createIndicator(float weight, float alpha, float duration, vec3 tint, float distance) {
//	float r = distance * interval / (2.f * duration);
//	r = clamp(r, 0.0f, 1.f);
//	r = ceil(r - floor(r) - 1.f + weight);
//	
//	fragColor = mix(fragColor, vec4(tint, 1.f), r * alpha);
//}

void createIndicator(float weight, float alpha, float radius, vec3 tint, float distance) {
	float delta = clamp(ceil((weight - abs(distance - radius - weight)) / viewSize), 0.f, 1.f);
	fragColor = mix(fragColor, vec4(tint, 1.f), delta * alpha);
}

void main() {
	if (useTex)
		fragColor = texture(tex, vs_out.texCoord) * vs_out.color;	
	else
		fragColor = vs_out.color;	
	
	if (fragColor.a < 0.01)
		discard;

	vec3 delta = vs_out.worldPosition.xyz - playerPosition;
	float radius = length(delta.xy);
	float radial = 0.5f * radius; 

	for (int i = 0; i < spreadCount; ++i) {
		float v = spreadWeight - min(abs(radius - (et - spreads[i].et) * speed), spreadWeight);
		fragColor.rgb += v * spreadAlpha * spreads[i].tint.rgb * fragColor.rgb;
	}

	float r = indicatorRadius - thresholdRadius;
	float count = round(1.f / interval) - 1.f;	

	createIndicator(indicatorWeight, indicatorAlpha, r * bt + thresholdRadius, indicatorTint, radial);
	for (float i = 1; i < count; ++i) {
		createIndicator(indicatorWeight, indicatorAlpha, r * (bt + interval * i) + thresholdRadius, indicatorTint, radial);
	}
	createIndicator(indicatorWeight, indicatorAlpha * (1.f - bt / interval), r * (bt + interval * count) + thresholdRadius, indicatorTint, radial);

	// threshold
	createIndicator(thresholdWeight, thresholdAlpha, thresholdRadius, thresholdTint, radial);

	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        brightColor = vec4(fragColor.rgb, 1.0);
//		fragColor.rgb = vec3(1.0);
	} else {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
