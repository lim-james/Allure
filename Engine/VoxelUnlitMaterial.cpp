#include "VoxelUnlitMaterial.h"

Material::VoxelUnlit::VoxelUnlit() {
	shader = new Shader("Files/Shaders/unlitVX.vert", "Files/Shaders/unlitVX.frag");
}

void Material::VoxelUnlit::SetAttributes() {}
