#include "InvertMaterial.h"

Material::Invert::Invert() {
	shader = new Shader("Files/Shaders/standard2D.vert", "Files/Shaders/invert2D.frag");
	shader->Use();
	shader->SetInt("tex", 0);
}

void Material::Invert::SetAttributes() {
}
