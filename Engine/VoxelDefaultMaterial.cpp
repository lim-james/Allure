#include "VoxelDefaultMaterial.h"

Material::VoxelDefault::VoxelDefault() {
	shader = new Shader("Files/Shaders/standardVX.vert", "Files/Shaders/standardVX.frag");
}

void Material::VoxelDefault::SetAttributes() {}
