#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 brightColor;

//out vec4 color;

struct Material {
	bool useMap;
	sampler2D map;
	vec4 tint;
};

in VS_OUT {
	vec3 fragmentPosition;
	vec3 normal;
	vec2 texCoord;
} vs_out;

uniform Material material;
uniform float alphaClipping;

vec4 getBrightColor(vec4 fragColor) {
	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        return vec4(fragColor.rgb, 1.0);
    else
        return vec4(0.0, 0.0, 0.0, 1.0);
}

void main() {
	if (material.useMap) {
		color = texture(material.map, vs_out.texCoord) * material.tint;
	} else {
		color = material.tint;
	}

	if (color.a < alphaClipping) discard;

	brightColor = getBrightColor(color);
}