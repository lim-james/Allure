#include "SpriteDefaultMaterial.h"

Material::SpriteDefault::SpriteDefault() {
	shader = new Shader("Files/Shaders/standard2D.vert", "Files/Shaders/standard2D.frag");
	shader->Use();
	shader->SetInt("tex", 0);
}

void Material::SpriteDefault::SetAttributes() {
}
