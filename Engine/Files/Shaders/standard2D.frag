#version 330 core

out vec4 color;

in VS_OUT {
	vec2 texCoord;
	vec4 color;
} vs_out;

uniform bool useTex;
uniform sampler2D tex;

void main() {
	if (useTex)
		color = texture(tex, vs_out.texCoord) * vs_out.color;	
	else
		color = vs_out.color;	

	if (length(color.a) < 0.01)
		discard;
}
