#include "TilemapDefaultMaterial.h"

Material::TilemapDefault::TilemapDefault() {
	flags = 0;
	shader = new Shader("Files/Shaders/standard2D.vert", "Files/Shaders/standard2D.frag");
	shader->Use();
	shader->SetInt("tex", 0);
}

void Material::TilemapDefault::SetAttributes() {}
