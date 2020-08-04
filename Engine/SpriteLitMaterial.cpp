#include "SpriteLitMaterial.h"

Material::SpriteLit::SpriteLit() {
	flags = 0;
	shader = new Shader("Files/Shaders/lit2D.vert", "Files/Shaders/lit2D.frag");
	shader->Use();
	shader->SetInt("tex", 0);
}

void Material::SpriteLit::SetAttributes() {}
