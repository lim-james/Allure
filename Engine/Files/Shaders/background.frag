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
uniform float spread;
uniform vec3 spreadTint;

uniform vec3 indicatorTint;
uniform float viewSize;
uniform float interval;
uniform float bt;

uniform vec3 thresholdTint;
uniform float threshold;

uniform vec3 playerPosition;

uniform bool useTex;
uniform sampler2D tex;

void createIndicator(float duration, vec3 tint, float distance) {
	float r = distance * interval / (2.f * duration);
	r = clamp(r, 0.0f, 1.f);
	r = ceil(r - floor(r) - 0.95f);
	
	fragColor = mix(fragColor, vec4(tint, 1.f), r * 0.5f);
}

void main() {
	if (useTex)
		fragColor = texture(tex, vs_out.texCoord) * vs_out.color;	
	else
		fragColor = vs_out.color;	

	if (length(fragColor.a) < 0.01)
		discard;

	vec3 delta = vs_out.worldPosition.xyz - playerPosition;
	float radial = length(delta.xy);

	float v = spread - min(abs(radial - et * speed), spread);
	fragColor.rgb += v * spreadTint.rgb * fragColor.rgb;

	float square = max(abs(delta.x), abs(delta.y)) / viewSize;

	for (float i = 0; i < 2.f; ++i) {
		createIndicator(bt + i * interval, indicatorTint, square);
	}

	// threshold
	// createIndicator(threshold * 0.75f, thresholdTint, square);
	
	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        brightColor = vec4(fragColor.rgb, 1.0);
//		fragColor.rgb = vec3(1.0);
	} else {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
