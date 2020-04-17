#include "MeshDefaultMaterial.h"

Material::MeshDefault::MeshDefault() {
	shader = new Shader("Files/Shaders/standard3D.vert", "Files/Shaders/standard3D.frag");
}

void Material::MeshDefault::SetAttributes() {
}
