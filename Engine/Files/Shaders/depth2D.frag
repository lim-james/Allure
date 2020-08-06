#version 330 core

out vec4 color;

in vec2 texCoord;

uniform bool useTex;
uniform sampler2D tex;

void main() {
	if (useTex) {
		color = texture(tex, texCoord);	
		if (color.a < 0.01)
			discard;
	}
}
