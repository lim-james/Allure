#version 330 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

struct Light {
	int type;

	float range;
	float innerCutOff;
	float outerCutOff;

	vec3 color;
	float intensity;

	float strength;

	bool castShadows;

	vec3 position;
	vec3 direction;
};

in VS_OUT {
	vec3 worldPosition;
	vec3 normal;
	vec2 texCoord;
	vec2 screenUV;
	vec2 lightUVs[8];
	vec4 color;
} vs_out;

uniform bool useTex;
uniform sampler2D tex;

uniform int lightCount;
uniform Light lights[8];
uniform sampler2D shadowMap;

float calculateShadow(vec2 uv, vec2 targetUV, float stride, Light light) {
	vec2 point = uv;
	vec2 dir = normalize(uv - targetUV) * stride;

	float kBias = 0.00001f;
	float depth = texture(shadowMap, uv).r;

	if (gl_FragCoord.z > depth) {
		return 1.f;
	}

	while (dot(dir, point - targetUV) >= 0.f) {
		point -= dir;
		float curr = texture(shadowMap, point).r;

		if (curr < depth - kBias) {
			return 1.f;	
		}
	}

	return 0.f;
}

void main() {
	if (useTex)
		fragColor = texture(tex, vs_out.texCoord) * vs_out.color;	
	else
		fragColor = vs_out.color;	

	if (fragColor.a < 0.01)
		discard;

	vec2 stride = 1.f / textureSize(shadowMap, 0); // gets size of single texel

	vec3 color = fragColor.rgb;
	vec3 Lo = vec3(0.0);

    for (int i = 0; i < lightCount; ++i) {
		vec3 lightPosition = lights[i].position;

        // calculate per-light radiance
		vec3 D;
        float dist;
		float brightness = 1.0f;

		if (lights[i].type == 0) {
			// spot light
			D = lightPosition - vs_out.worldPosition;
			dist = length(D);
			float theta = dot(normalize(D), normalize(-lights[i].direction));
			float epsilon = lights[i].innerCutOff - lights[i].outerCutOff;
			brightness = max(lights[i].range - dist, 0.0f) / lights[i].range;
			brightness *= clamp((theta - lights[i].outerCutOff) / epsilon, 0.0, 1.0);
		} else if (lights[i].type == 1) {
			// directional light
			D = -lights[i].direction;
			dist = 1;
		} else {
			// point light
			D = lightPosition - vs_out.worldPosition;
			dist = length(D);
		}

		float shadow = calculateShadow(vs_out.screenUV, vs_out.lightUVs[i], length(stride), lights[i]);
		shadow *= lights[i].strength;	

        float attenuation = min(lights[i].intensity / (dist * dist), 1.f) * (1.f - shadow);
        vec3 radiance = vec3(lights[i].color) * attenuation;

        // add to outgoing radiance Lo
        Lo += color * radiance * brightness;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}

	fragColor.rgb = Lo;

	color = fragColor.rgb * fragColor.a;
	float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        brightColor = vec4(color, 1.0);
	} else {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
