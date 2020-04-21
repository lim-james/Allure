#include "MeshDefaultMaterial.h"

#include <GL/glew.h>

Material::MeshDefault::MeshDefault()
	: useAlbedoMap(false), albedoMap(0), albedo(1.0f)
	, useNormalMap(false), normalMap(0), normal(0.f)
	, useMetallicMap(false), metallicMap(0), metallic(0)
	, useRoughnessMap(false), roughnessMap(0), roughness(0)
	, useAOMap(false), aoMap(0), ao(1)
{
	shader = new Shader("Files/Shaders/standard3D.vert", "Files/Shaders/standard3D.frag");
	shader->Use();
	shader->SetInt("material.albedoMap", 0);
	shader->SetInt("material.normalMap", 1);
	shader->SetInt("material.metallicMap", 2);
	shader->SetInt("material.roughnessMap", 3);
	shader->SetInt("material.aoMap", 4);
}

void Material::MeshDefault::SetAttributes() {
	shader->SetInt("material.useAlbedoMap", useAlbedoMap);
	if (useAlbedoMap) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedoMap);
	} else {
		shader->SetVector4("material.albedo", albedo);
	}

	shader->SetInt("material.useNormalMap", useNormalMap);
	if (useNormalMap) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap);
	} else {
		shader->SetVector4("material.normal", normal);
	}

	shader->SetInt("material.useMetallicMap", useMetallicMap);
	if (useMetallicMap) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallicMap);
	} else {
		shader->SetFloat("material.metallic", metallic);
	}

	shader->SetInt("material.useRoughnessMap", useRoughnessMap);
	if (useRoughnessMap) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughnessMap);
	} else {
		shader->SetFloat("material.roughness", roughness);
	}

	shader->SetInt("material.useAOMap", useAOMap);
	if (useAOMap) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, aoMap);
	} else {
		shader->SetFloat("material.ao", ao);
	}
}
