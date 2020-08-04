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
	vec2 texCoord;
	vec4 color;
} vs_out;

uniform bool useTex;
uniform sampler2D tex;

uniform Light lights[8];
uniform int lightCount;

void main() {
	if (useTex)
		fragColor = texture(tex, vs_out.texCoord) * vs_out.color;	
	else
		fragColor = vs_out.color;	

	if (fragColor.a < 0.01)
		discard;

	vec3 color = fragColor.rgb * fragColor.a;

    for (int i = 0; i < lightCount; ++i) {
		vec3 lightPosition = lights[i].position;

        // calculate per-light radiance
		vec3 D;
        float dist;
		float intensity = lights[i].intensity;
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

        vec3 L = normalize(D);
        vec3 H = normalize(V + L);

		float shadow = calculateShadow(vs_out.fragPosLightSpace.positions[i], lights[i]);
		shadow *= lights[i].strength;	

        float attenuation = intensity / (dist * dist) * (1.0 - shadow);
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
        Lo +=  (kD * albedo / PI + specular) * brightness * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again

	}

	float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        brightColor = vec4(color, 1.0);
	} else {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
