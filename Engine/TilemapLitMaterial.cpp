#include "TilemapLitMaterial.h"

Material::TilemapLit::TilemapLit() {
	flags = RENDER_DEPTH;
	shader = new Shader("Files/Shaders/lit2D.vert", "Files/Shaders/lit2D.frag");
	shader->Use();
	shader->SetInt("tex", 0);
	shader->SetInt("shadowMap", 1);
}

void Material::TilemapLit::SetAttributes() {}
