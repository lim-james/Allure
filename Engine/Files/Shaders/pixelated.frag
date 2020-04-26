#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D tex;
uniform float size;

float sround(float value, float segments, float base) {
	float invBase = 1.f - base;
	return (round(value * segments) / segments) * invBase + base;
}

vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
	vec2 resultCoord = vec2(ivec2(texCoord * size)) / size + .5f / size;
	vec3 result = texture(tex, resultCoord).rgb;
	color = vec4(result, 1.0);
//	vec3 hsvResult = rgb2hsv(result);
//	hsvResult.z = sround(hsvResult.z, 4, 0.2f);
//	color = vec4(hsv2rgb(hsvResult), 1.f);
}