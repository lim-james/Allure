#include "CircleMaterial.h"

Material::Circle::Circle()
	: borderWeight(0.1f) {
	shader = new Shader("Files/Shaders/standard2D.vert", "Files/Shaders/circle.frag");
}

void Material::Circle::SetAttributes() {
	shader->SetFloat("borderWeight", borderWeight);
}
