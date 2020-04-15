#include "LineDefaultMaterial.h"

Material::LineDefault::LineDefault() {
	shader = new Shader("Files/Shaders/line.vert", "Files/Shaders/standard2D.frag");
}

void Material::LineDefault::SetAttributes() {
}
