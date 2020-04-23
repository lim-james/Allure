#include "IBLMaterial.h"

#include <GL/glew.h>

Material::IBL::IBL()
	: useAlbedoMap(false), albedoMap(0), albedo(1.0f)
	, useNormalMap(false), normalMap(0)
	, useMetallicMap(false), metallicMap(0), metallic(0)
	, useRoughnessMap(false), roughnessMap(0), roughness(0)
	, useAOMap(false), aoMap(0), ao(1)
	, refractiveIndex(1.52f), environment(0)
	, tiling(1.f), offset(0.f) {
	shader = new Shader("Files/Shaders/IBL_3D.vert", "Files/Shaders/IBL_3D.frag");
	shader->Use();
	shader->SetInt("material.albedoMap", 0);
	shader->SetInt("material.normalMap", 1);
	shader->SetInt("material.metallicMap", 2);
	shader->SetInt("material.roughnessMap", 3);
	shader->SetInt("material.aoMap", 4);
	shader->SetInt("material.environment", 5);
}

void Material::IBL::SetAttributes() {
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

	shader->SetFloat("material.refractiveIndex", refractiveIndex);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, environment);

	shader->SetVector2("tiling", tiling);
	shader->SetVector2("offset", offset);
}
