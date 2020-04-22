#include "SampleScene.h"

// component
#include "Camera.h"
#include "Light.h"
#include "EditorCamera.h"
#include "SpriteRender.h"
#include "MeshRender.h"
#include "Orbit.h"
// materials
#include "MeshDefaultMaterial.h"
#include "UnlitDefaultMaterial.h"
// Utils
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "Layers.h"

void SampleScene::Awake() {
	Scene::Awake();
}

void SampleScene::Create() {
	Scene::Create();

	Material::UnlitDefault* opaque = new Material::UnlitDefault;
	opaque->alphaClipping = 0.01f;

	Material::MeshDefault* rustediron = new Material::MeshDefault;
	rustediron->useAlbedoMap = true;
	rustediron->albedoMap = Load::TGA("Files/Textures/rustediron2_basecolor.tga");
	rustediron->useNormalMap = true;
	rustediron->normalMap = Load::TGA("Files/Textures/rustediron2_normal.tga");
	rustediron->useMetallicMap = true;
	rustediron->metallicMap = Load::TGA("Files/Textures/rustediron2_metallic.tga");
	rustediron->useRoughnessMap = true;
	rustediron->roughnessMap = Load::TGA("Files/Textures/rustediron2_roughness.tga");
	rustediron->ao = 0.5f;

	Material::MeshDefault* streakedmetal = new Material::MeshDefault;
	streakedmetal->useAlbedoMap = true;
	streakedmetal->albedoMap = Load::TGA("Files/Textures/streakedmetal-albedo.tga");
	streakedmetal->useMetallicMap = true;
	streakedmetal->metallicMap = Load::TGA("Files/Textures/streakedmetal-metalness.tga");
	streakedmetal->useRoughnessMap = true;
	streakedmetal->roughnessMap = Load::TGA("Files/Textures/streakedmetal-roughness.tga");
	streakedmetal->ao = 0.5f;

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->clearColor = 0.9f;
	camera->cullingMask -= UI;

	{
		EditorCamera* const editorCamera = entities->AddComponent<EditorCamera>(mainCamera);
		editorCamera->SetActive(true);

		Light* const light = entities->AddComponent<Light>(mainCamera);
		light->SetActive(true);
		light->intensity = 100.0f;
		light->outerCutOff = 20.f;
		light->range = 10.0f;
		light->type = LIGHT_SPOT;
	}

	const unsigned directionalLight = entities->Create();
	{
		Transform* const transform = entities->GetComponent<Transform>(directionalLight);
		transform->rotation.x = -90.0f;
		transform->UpdateAxes();
		
		Light* const light = entities->AddComponent<Light>(directionalLight);
		light->SetActive(true);
		light->color = vec4f(253.f / 255.f, 251.f / 255.f, 211.f / 255.f, 1.0f);
		light->intensity = 1.0;
		light->type = LIGHT_DIRECTIONAL;
	}

	const unsigned pointLight = entities->Create();
	{
		Transform* const transform = entities->GetComponent<Transform>(pointLight);
		transform->scale = 0.2f;

		Light* const light = entities->AddComponent<Light>(pointLight);
		light->SetActive(true);
		light->color = vec4f(253.f / 255.f, 251.f / 255.f, 211.f / 255.f, 1.0f);
		light->intensity = 1.0;
		light->type = LIGHT_POINT;

		MeshRender* const render = entities->AddComponent<MeshRender>(pointLight);
		render->SetActive(true);
		render->SetMaterial(opaque);
		render->SetModel(Load::OBJ("Files/Models/sphere.obj"));
	}

	Orbit* const orbit = entities->AddComponent<Orbit>(pointLight);
	orbit->SetActive(true);
	orbit->radius = 1.5f;

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetDynamic(false);

		orbit->orbit = transform;

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(rustediron);
		render->SetModel(Load::OBJ("Files/Models/sphere.obj"));
		render->SetDynamic(false);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->scale = vec3f(10.0f, 1.0f, 10.0f);
		transform->translation.y = -3.f;
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(streakedmetal);
		render->SetModel(Load::OBJ("Files/Models/cube.obj"));
		render->SetDynamic(false);
	}
}
