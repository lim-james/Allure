#include "PortalMaterial.h"

#include <GL/glew.h>

Material::Portal::Portal() : texture(0) {
	shader = new Shader("Files/Shaders/portal.vert", "Files/Shaders/portal.frag");
	shader->Use();
	shader->SetInt("portalView", 0);
}

void Material::Portal::SetAttributes() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}
