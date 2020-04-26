#version 330 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

struct Material {
	bool useMap;
	sampler2D map;
	vec4 tint;
};

in VS_OUT {
	vec3 normal;
	vec2 texCoord;
} vs_out;

uniform Material material;
uniform float alphaClipping;

vec4 getBrightColor(vec4 fragColor) {
	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.9)
        return vec4(fragColor.rgb, 1.0);
    else
        return vec4(0.0, 0.0, 0.0, 1.0);
}

void main() {
	if (material.useMap) {
		fragColor = texture(material.map, vs_out.texCoord) * material.tint;
	} else {
		fragColor = material.tint;
	}

	if (fragColor.a < alphaClipping) discard;

	brightColor = getBrightColor(fragColor);
}