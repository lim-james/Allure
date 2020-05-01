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

struct LightSpacePoints {
	vec4 positions[16];
};

in VS_OUT {
	vec3 worldPosition;
	vec3 normal;
	vec4 color;
	LightSpacePoints fragPosLightSpace;
} fs_in;

uniform vec3 viewPosition;

uniform Light lights[16];
uniform int lightCount;

float calculateShadow(vec4 fragPosLightSpace, Light light) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    float closestDepth = texture(light.shadowMap, projCoords.xy).r; 

	float kBias = 0.00001;
	float bias = max(kBias * (1.0 - dot(fs_in.normal, -light.direction)), kBias);  

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
	const int depth = 1;
	for(int x = -depth; x <= depth; ++x) {
		for(int y = -depth; y <= depth; ++y) {
			float pcfDepth = texture(light.shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.f : 0.0;        
		}    
	}
	shadow /= (depth * 2 + 1) * (depth * 2 + 1);

	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec4 getBrightColor(vec4 fragColor) {
	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        return vec4(fragColor.rgb, 1.0);
    else
        return vec4(0.0, 0.0, 0.0, 1.0);
}

void main() {
	vec3 Lo = vec3(0.f);
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

		float shadow = calculateShadow(fs_in.fragPosLightSpace.positions[i], lights[i]);
		shadow *= lights[i].strength;	

        float attenuation = intensity / (dist * dist) * (1.f - shadow);
        vec3 radiance = vec3(lights[i].color) * attenuation;

		float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += fs_in.color.rgb * brightness * radiance * NdotL;  	
	}
	
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = 0.03f * fs_in.color.rgb;
    
    vec3 color = ambient + Lo;
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

	fragColor = vec4(color, fs_in.color.a);
	brightColor = getBrightColor(fragColor);
}
