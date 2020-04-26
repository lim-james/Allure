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

	vec3 position;
	vec3 direction;

	sampler2D shadowMap;
};

in VS_OUT {
	vec3 worldPosition;
	vec3 normal;
	vec4 color;
} fs_in;

uniform vec3 viewPosition;

uniform Light lights[16];
uniform int lightCount;

vec4 getBrightColor(vec4 fragColor) {
	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        return vec4(fragColor.rgb, 1.0);
    else
        return vec4(0.0, 0.0, 0.0, 1.0);
}

void main() {

	vec3 result = vec3(0.f);
	vec3 N = normalize(fs_in.normal);
    vec3 V = normalize(viewPosition - fs_in.worldPosition);

	for (int i = 0; i < lightCount; ++i) {
		vec3 D;
        float dist;
		float intensity = lights[i].intensity;
		float brightness = 1.0f;

		if (lights[i].type == 0) {
			// spot light
			D = lights[i].position - fs_in.worldPosition;
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
			D = lights[i].position - fs_in.worldPosition;
			dist = length(D);
		}

        vec3 L = normalize(D);
        vec3 H = normalize(V + L);

        float attenuation = intensity / (dist * dist);
        vec3 radiance = vec3(lights[i].color) * attenuation;

		float NdotL = max(dot(N, L), 0.0);        
        // add to outgoing radiance Lo
        result += fs_in.color.rgb * brightness * radiance * NdotL;  	
	}
	
	result += fs_in.color.rgb * 0.3f;	

	fragColor = vec4(result, fs_in.color.a);
	brightColor = getBrightColor(fragColor);
}
