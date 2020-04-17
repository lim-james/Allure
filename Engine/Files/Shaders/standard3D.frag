#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 brightColor;

//out vec4 color;

struct Material {
	sampler2D albedo;
	vec3 tint;

	sampler2D metallic;
	float smoothness;
};

struct Light {
	int type;

	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float power;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;

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

uniform float near;
uniform float far;

uniform vec3 viewPosition;
uniform Material material;

uniform Light lights[16];
uniform int lightCount;

float sround(float value, float segments) {
	return round(value * segments) / segments;
}

float linearDepth(float depth) {
	float z = depth * 2.f - 1.f;
	return (2.f * near * far) / (far + near - z * (far - near));
}

float calculateShadow(vec4 fragPosLightSpace, Light light) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    float closestDepth = texture(light.shadowMap, projCoords.xy).r; 

	float kBias = 0.00001;
	float bias = max(kBias * (1.0 - dot(vs_out.normal, -light.direction)), kBias);  

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

float calcDiff(vec3 normal, vec3 lightDirection) {
	return max(dot(normal, lightDirection), 0);
}

float calcSpec(vec3 viewDirection, vec3 reflectDirection) {
	return pow(max(dot(viewDirection, reflectDirection), 0), material.smoothness);
}

vec3 calcDirectionalLight(vec4 fragPosLightSpace, Light light, vec3 normal, vec3 viewDirection, vec3 materialPoint, vec3 specularPoint) {
	vec3 lightDirection		= normalize(-light.direction);
	vec3 reflectDirection	= reflect(-lightDirection, normal);

	float diff = calcDiff(normal, lightDirection);
	float spec = calcSpec(viewDirection, reflectDirection);
	
	vec3 ambient	= light.ambient * materialPoint;
	vec3 diffuse	= diff * light.diffuse * materialPoint;
	vec3 specular	= spec * light.specular * specularPoint;

	float shadow = calculateShadow(fragPosLightSpace, light);

//	return ambient + diffuse + specular;
	return ambient + (1.0f - shadow) * (diffuse + specular);
//	return vec3(shadow);
}

vec3 calcPointLight(Light light, vec3 normal, vec3 viewDirection, vec3 materialPoint, vec3 specularPoint) {
	vec3 lightDirection		= normalize(light.position - vs_out.fragmentPosition);
	vec3 reflectDirection	= reflect(-lightDirection, normal);

	float diff = calcDiff(normal, lightDirection);
	float spec = calcSpec(viewDirection, reflectDirection);
	
	float dist			= length(light.position - vs_out.fragmentPosition);
	float attenuation	= light.power / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient	= attenuation * light.ambient * materialPoint;
	vec3 diffuse	= attenuation * diff * light.diffuse * materialPoint;
	vec3 specular	= attenuation * spec * light.specular * specularPoint;
	
	return ambient + diffuse + specular;
}

vec3 calcSpotLight(vec4 fragPosLightSpace, Light light, vec3 normal, vec3 viewDirection, vec3 materialPoint, vec3 specularPoint) {
	vec3 lightDirection		= normalize(light.position - vs_out.fragmentPosition);
	vec3 reflectDirection	= reflect(-lightDirection, normal);

	float diff = calcDiff(normal, lightDirection);
	float spec = calcSpec(viewDirection, reflectDirection);
		
	float theta		= dot(lightDirection, normalize(-light.direction));
	float epsilon	= light.cutOff - light.outerCutOff;
	float intensity	= clamp((theta - light.outerCutOff) / epsilon, 0, 1.f);

	float dist			= length(light.position - vs_out.fragmentPosition);
	float attenuation	= light.power / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient	= attenuation * light.ambient * materialPoint;
	vec3 diffuse	= attenuation * intensity * diff * light.diffuse * materialPoint;
	vec3 specular	= attenuation * intensity * spec * light.specular * specularPoint;

	float shadow = calculateShadow(fragPosLightSpace, light);

//	return ambient + diffuse + specular;
	return ambient + (1.0f - shadow) * (diffuse + specular);
//	return vec3(shadow);
}

vec4 getBrightColor(vec4 fragColor) {
	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        return vec4(fragColor.rgb, 1.0);
    else
        return vec4(0.0, 0.0, 0.0, 1.0);
}

void main() {
	color = vec4(1.0);
	return;

	vec3 viewDirection = normalize(viewPosition - vs_out.fragmentPosition);

	vec4 diffuse = texture(material.albedo, vs_out.texCoord);
	vec3 materialPoint = diffuse.rgb * material.tint;
	vec3 specularPoint = texture(material.metallic, vs_out.texCoord).rgb;

	vec3 result = vec3(0.f);

	vec3 normal = normalize(vs_out.normal);

	for (int i = 0; i < lightCount; ++i) {
		if (lights[i].type == 0) {
			result += calcPointLight(lights[i], normal, viewDirection, materialPoint, specularPoint);
		} else if (lights[i].type == 1) {
			result += calcDirectionalLight(vs_out.fragPosLightSpace.positions[i], lights[i], normal, viewDirection, materialPoint, specularPoint);
		} else {
			result += calcSpotLight(vs_out.fragPosLightSpace.positions[i], lights[i], normal, viewDirection, materialPoint, specularPoint);
		}
	}

//	color = vec4(vec3(1.0f - linearDepth(gl_FragCoord.z) / far), 1.0f);
//	float outline = 1 - clamp(dot(viewDirection, normal), 0, 1);
//	outline *= outline;
//	outline = smoothstep(0.0, 0.5, outline);

	color = vec4(result, diffuse.a);
//	color = vec4(vec3(outline), diffuse.a);

	brightColor = getBrightColor(color);
}