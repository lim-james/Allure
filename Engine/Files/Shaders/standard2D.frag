#version 330 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

in VS_OUT {
	vec2 texCoord;
	vec4 color;
} vs_out;

uniform bool useTex;
uniform sampler2D tex;

void main() {
	if (useTex)
		fragColor = texture(tex, vs_out.texCoord) * vs_out.color;	
	else
		fragColor = vs_out.color;	

	if (length(fragColor.a) < 0.01)
		discard;

	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        brightColor = vec4(fragColor.rgb, 1.0);
	} else {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
