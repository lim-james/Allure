#include "UnlitDefaultMaterial.h"

#include <GL/glew.h>

Material::UnlitDefault::UnlitDefault() {
	shader = new Shader("Files/Shaders/unlit3D.vert", "Files/Shaders/unlit3D.frag");
	shader->Use();
	shader->SetInt("material.map", 0);

	alphaClipping = 0.5f;

	texture = 0;
	tint = vec4f(1.0f);

	tiling = vec2f(1.0f);
	offset = vec2f(0.0f);
}

void Material::UnlitDefault::SetAttributes() {
	shader->SetFloat("alphaClipping", alphaClipping);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader->SetInt("material.useMap", texture);
	shader->SetVector4("material.tint", tint);
	shader->SetVector2("tiling", tiling);
	shader->SetVector2("offset", offset);
}
