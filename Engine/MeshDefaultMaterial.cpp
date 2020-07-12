#include "MeshDefaultMaterial.h"

#include "EngineProperties.h"

#include <GL/glew.h>

Material::MeshDefault::MeshDefault()
	: useAlbedoMap(false), albedoMap(0), albedo(1.0f)
	, useNormalMap(false), normalMap(0)
	, useMetallicMap(false), metallicMap(0), metallic(0)
	, useRoughnessMap(false), roughnessMap(0), roughness(0)
	, useAOMap(false), aoMap(0), ao(1)
	, tiling(1.f), offset(0.f)
	, alphaClipping(0.5f) {
	shader = new Shader("Files/Shaders/standard3D.vert", "Files/Shaders/standard3D.frag");

	//shader = new Shader("Files/Shaders/depth3D.vert", "Files/Shaders/depth3D.frag");
	shader->Use();

	for (unsigned i = 0; i < MAX_LIGHTS; ++i) {
		shader->SetInt("lights[" + std::to_string(i) + "].shadowMap", i);
	}

	shader->SetInt("material.albedoMap", MAX_LIGHTS);
	shader->SetInt("material.normalMap", MAX_LIGHTS + 1);
	shader->SetInt("material.metallicMap", MAX_LIGHTS + 2);
	shader->SetInt("material.roughnessMap", MAX_LIGHTS + 3);
	shader->SetInt("material.aoMap", MAX_LIGHTS + 4);
}

void Material::MeshDefault::SetAttributes() {
	const unsigned textureID = GL_TEXTURE0 + MAX_LIGHTS;

	shader->SetInt("material.useAlbedoMap", useAlbedoMap);
	if (useAlbedoMap) {
		glActiveTexture(textureID);
		glBindTexture(GL_TEXTURE_2D, albedoMap);
	} else {
		shader->SetVector4("material.albedo", albedo);
	}

	shader->SetInt("material.useNormalMap", useNormalMap);
	if (useNormalMap) {
		glActiveTexture(textureID + 1);
		glBindTexture(GL_TEXTURE_2D, normalMap);
	}

	shader->SetInt("material.useMetallicMap", useMetallicMap);
	if (useMetallicMap) {
		glActiveTexture(textureID + 2);
		glBindTexture(GL_TEXTURE_2D, metallicMap);
	} else {
		shader->SetFloat("material.metallic", metallic);
	}

	shader->SetInt("material.useRoughnessMap", useRoughnessMap);
	if (useRoughnessMap) {
		glActiveTexture(textureID + 3);
		glBindTexture(GL_TEXTURE_2D, roughnessMap);
	} else {
		shader->SetFloat("material.roughness", roughness);
	}

	shader->SetInt("material.useAOMap", useAOMap);
	if (useAOMap) {
		glActiveTexture(textureID + 4);
		glBindTexture(GL_TEXTURE_2D, aoMap);
	} else {
		shader->SetFloat("material.ao", ao);
	}

	shader->SetVector2("tiling", tiling);
	shader->SetVector2("offset", offset);
	
	shader->SetFloat("alphaClipping", alphaClipping);
}
