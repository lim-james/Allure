#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D tex;
uniform bool horizontal;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {             
     vec2 texOffset = 2.0 / textureSize(tex, 0); // gets size of single texel
     vec3 result = texture(tex, texCoord).rgb * 0.227;

	 int size = 5;

     if (horizontal) {
         for(int i = 1; i < size; ++i) {
			float blur = weight[i];//1.f / float(i * size);
            result += texture(tex, texCoord + vec2(texOffset.x * i, 0.0)).rgb * blur;
            result += texture(tex, texCoord - vec2(texOffset.x * i, 0.0)).rgb * blur;
         }
     } else {
         for(int i = 1; i < size; ++i) {
			float blur = weight[i];//1.f / float(i * size);
            result += texture(tex, texCoord + vec2(0.0, texOffset.y * i)).rgb * blur;
            result += texture(tex, texCoord - vec2(0.0, texOffset.y * i)).rgb * blur;
         }
     }

	color = vec4(result, 1.0f);
}