#version 330 core

out vec4 color;

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
	
	color = mix(color, vec4(tint, 1.f), r * 0.5f);
}

void main() {
	if (useTex)
		color = texture(tex, vs_out.texCoord) * vs_out.color;	
	else
		color = vs_out.color;	

	if (length(color.a) < 0.01)
		discard;

	vec3 delta = vs_out.worldPosition.xyz - playerPosition;
	float radial = length(delta.xy);

	float v = spread - min(abs(radial - et * speed), spread);
	color.rgb += v * spreadTint * color.rgb;

	float square = max(abs(delta.x), abs(delta.y)) / viewSize;

	for (float i = 0; i < 2.f; ++i) {
		createIndicator(bt + i * interval, indicatorTint, square);
	}

	// threshold
	createIndicator(threshold * 0.75f, thresholdTint, square);
}
