#version 330 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

in vec4 position;
//in vec2 texCoord;

uniform sampler2D portalView;

vec4 getBrightColor(vec4 fragColor) {
	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        return vec4(fragColor.rgb, 1.0);
    else
        return vec4(0.0, 0.0, 0.0, 1.0);
}

void main() {

	vec2 texCoord = ((position.xy / position.w) + 1.f) * 0.5f;
//	fragColor = vec4(texCoord, 0, 1);
	fragColor = texture(portalView, texCoord);
	brightColor = getBrightColor(fragColor);
}