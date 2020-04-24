#include "VoxelDefaultMaterial.h"

Material::VoxelDefault::VoxelDefault() {
	shader = new Shader("Files/Shaders/voxel.vert", "Files/Shaders/voxel.frag");
}

void Material::VoxelDefault::SetAttributes() {}
