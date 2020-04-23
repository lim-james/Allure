#version 330 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

struct Material {
	bool useAlbedoMap;
	sampler2D albedoMap;
	vec4 albedo;

	bool useNormalMap;
	sampler2D normalMap;

	bool useMetallicMap;
	sampler2D metallicMap;
	float metallic;

	bool useRoughnessMap;
	sampler2D roughnessMap;
	float roughness;

	bool useAOMap;
	sampler2D aoMap;
	float ao;

	float refractiveIndex;
	samplerCube environment;
};

struct Light {
	int type;

	float range;
	float innerCutOff;
	float outerCutOff;

	vec4 color;
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
	vec3 fragmentPosition;
	vec3 normal;
	vec2 texCoord;
	LightSpacePoints fragPosLightSpace;
} vs_out;

uniform vec3 viewPosition;
uniform Material material;

uniform Light lights[16];
uniform int lightCount;

const float PI = 3.14159265359;

vec3 getNormalFromMap() {
	vec3 normal = texture(material.normalMap, vs_out.texCoord).xyz;

    vec3 tangentNormal = normal * 2.0 - 1.0;

    vec3 Q1  = dFdx(vs_out.fragmentPosition);
    vec3 Q2  = dFdy(vs_out.fragmentPosition);
    vec2 st1 = dFdx(vs_out.texCoord);
    vec2 st2 = dFdy(vs_out.texCoord);

    vec3 N = normalize(vs_out.normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;
	
	float num = a2;
	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;

	return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec4 getBrightColor(vec4 fragColor) {
	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        return vec4(fragColor.rgb, 1.0);
    else
        return vec4(0.0, 0.0, 0.0, 1.0);
}

void main() {
	float alpha;
	vec3 albedo, N;
	float metallic, roughness, ao;

	if (material.useAlbedoMap) {
		vec4 color = texture(material.albedoMap, vs_out.texCoord);
		albedo = pow(color.rgb, vec3(2.2));
		alpha = color.a;
	} else {
		albedo = material.albedo.rgb;
		alpha = material.albedo.a;
	}

	if (material.useNormalMap) {
		N = getNormalFromMap();
	} else {
		N = normalize(vs_out.normal);
	}

	if (material.useMetallicMap) {
		metallic = texture(material.metallicMap, vs_out.texCoord).r;
	} else {
		metallic = material.metallic;
	}

	if (material.useRoughnessMap) {
		roughness = texture(material.roughnessMap, vs_out.texCoord).r;
	} else {
		roughness = material.roughness;	
	}

	if (material.useAOMap) {
		ao = texture(material.aoMap, vs_out.texCoord).r;
	} else {
		ao = material.ao;	
	}

    vec3 V = normalize(viewPosition - vs_out.fragmentPosition);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < lightCount; ++i) {
		vec3 lightPosition = lights[i].position;

        // calculate per-light radiance
		vec3 D;
        float dist;
		float intensity = lights[i].intensity;
		float brightness = 1.0f;

		if (lights[i].type == 0) {
			// spot light
			D = lightPosition - vs_out.fragmentPosition;
			dist = length(D);
			float theta = dot(normalize(D), normalize(-lights[i].direction));
			float epsilon = lights[i].innerCutOff - lights[i].outerCutOff;
			brightness = max(lights[i].range - dist, 0.0f) / lights[i].range;
			brightness *= clamp((theta - lights[i].outerCutOff) / epsilon, 0.0, 1.0);
		} else if (lights[i].type == 1) {
			// directional light
			D = lights[i].direction;
			dist = 1;
		} else {
			// point light
			D = lightPosition - vs_out.fragmentPosition;
			dist = length(D);
		}

        vec3 L = normalize(D);
        vec3 H = normalize(V + L);

        float attenuation = intensity / (dist * dist);
        vec3 radiance = vec3(lights[i].color) * attenuation;

        // Cook-Torrance BRDF
        float NDF = distributionGGX(N, H, roughness);   
        float G = geometrySmith(N, V, L, roughness);      
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 nominator = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = 1.0 - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * brightness * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
	
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = 0.03 * albedo * ao;

	float ratio = 1.00 / material.refractiveIndex;
    vec3 R = refract(-V, N, ratio);

	vec3 envColor = texture(material.environment, R).rgb;
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    fragColor = vec4(mix(envColor, color, alpha), 1.0);
	brightColor = getBrightColor(fragColor);
}