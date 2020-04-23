#include "SkyboxMaterial.h"

#include <GL/glew.h>

Material::Skybox::Skybox() : cubemap(0) {
	shader = new Shader("Files/Shaders/skybox.vert", "Files/Shaders/skybox.frag");
	shader->Use();
	shader->SetInt("skybox", 0);
}

void Material::Skybox::SetAttributes() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
}
