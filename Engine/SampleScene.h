#ifndef SAMPLE_SCENE_H
#define SAMPLE_SCENE_H

#include "Scene.h"
// materials
#include "MeshDefaultMaterial.h"
#include "MeshUnlitMaterial.h"
#include "IBLMaterial.h"
#include "SkyboxMaterial.h"

class SampleScene : public Scene {

	Material::MeshUnlit* opaque;
	Material::MeshDefault* rustediron;
	Material::MeshDefault* streakedmetal;
	Material::MeshDefault* hardwood;
	Material::MeshDefault* white;
	Material::MeshDefault* window;
	Material::Skybox* skybox;

public:

	void Awake() override;
	void Create() override;
	void Destroy() override;

};

#endif
