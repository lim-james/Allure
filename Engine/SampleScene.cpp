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
#include "LoadTexture.h"
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
	rustediron->albedoMap = Load::Texture("Files/Textures/rustediron2_basecolor.png");
	rustediron->useNormalMap = true;
	rustediron->normalMap = Load::Texture("Files/Textures/rustediron2_normal.png");
	rustediron->useMetallicMap = true;
	rustediron->metallicMap = Load::Texture("Files/Textures/rustediron2_metallic.png");
	rustediron->useRoughnessMap = true;
	rustediron->roughnessMap = Load::Texture("Files/Textures/rustediron2_roughness.png");
	rustediron->ao = 0.01f;

	Material::MeshDefault* streakedmetal = new Material::MeshDefault;
	streakedmetal->useAlbedoMap = true;
	streakedmetal->albedoMap = Load::Texture("Files/Textures/streakedmetal-albedo.png");
	streakedmetal->useMetallicMap = true;
	streakedmetal->metallicMap = Load::Texture("Files/Textures/streakedmetal-metalness.png");
	streakedmetal->useRoughnessMap = true;
	streakedmetal->roughnessMap = Load::Texture("Files/Textures/streakedmetal-roughness.png");
	streakedmetal->ao = 0.01f;

	Material::MeshDefault* white = new Material::MeshDefault;
	white->albedo = vec4f(1.f);
	white->ao = 0.1f;

	Model* const cube = Load::OBJ("Files/Models/cube.obj");
	Model* const sphere = Load::OBJ("Files/Models/sphere.obj");

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->clearColor = 0.9f;
	camera->cullingMask -= UI;

	{
		EditorCamera* const editorCamera = entities->AddComponent<EditorCamera>(mainCamera);
		editorCamera->SetActive(true);
	}

	// directional light
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->rotation.x = -90.0f;
		transform->UpdateAxes();
		
		Light* const light = entities->AddComponent<Light>(entity);
		light->SetActive(true);
		light->color = vec4f(253.f / 255.f, 251.f / 255.f, 211.f / 255.f, 1.0f);
		light->type = LIGHT_DIRECTIONAL;
	}

	// spot light
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->translation.x = 3.0f;
		transform->scale = 0.2f;
		transform->rotation.x = 90.f;
		transform->UpdateAxes();

		Light* const light = entities->AddComponent<Light>(entity);
		light->SetActive(true);
		light->intensity = 100.0f;
		light->outerCutOff = 30.f;
		light->range = 10.0f;
		light->type = LIGHT_SPOT;		
		
		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(opaque);
		render->SetModel(cube);
	}

	const unsigned pointLight = entities->Create();
	{
		Transform* const transform = entities->GetComponent<Transform>(pointLight);
		transform->scale = 0.2f;

		Light* const light = entities->AddComponent<Light>(pointLight);
		light->SetActive(true);
		light->color = vec4f(253.f / 255.f, 251.f / 255.f, 211.f / 255.f, 1.0f);
		light->intensity = 10.0;
		light->type = LIGHT_POINT;

		MeshRender* const render = entities->AddComponent<MeshRender>(pointLight);
		render->SetActive(true);
		render->SetMaterial(opaque);
		render->SetModel(sphere);
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
		render->SetModel(sphere);
		render->SetDynamic(false);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->scale = vec3f(10.0f, 1.0f, 10.0f);
		transform->translation.y = -5.f;
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(streakedmetal);
		render->SetModel(cube);
		render->SetDynamic(false);
	}

	//{
	//	const unsigned entity = entities->Create();

	//	Transform* const transform = entities->GetComponent<Transform>(entity);
	//	transform->scale = vec3f(10.0f, 1.0f, 10.0f);
	//	transform->translation.y = 5.f;
	//	transform->SetDynamic(false);

	//	MeshRender* const render = entities->AddComponent<MeshRender>(entity);
	//	render->SetActive(true);
	//	render->SetMaterial(white);
	//	render->SetModel(cube);
	//	render->SetDynamic(false);
	//}

	//{
	//	const unsigned entity = entities->Create();

	//	Transform* const transform = entities->GetComponent<Transform>(entity);
	//	transform->scale = vec3f(1.0f, 10.0f, 10.0f);
	//	transform->translation.x = -5.0f;
	//	transform->SetDynamic(false);

	//	MeshRender* const render = entities->AddComponent<MeshRender>(entity);
	//	render->SetActive(true);
	//	render->SetMaterial(white);
	//	render->SetModel(cube);
	//	render->SetDynamic(false);
	//}

	//{
	//	const unsigned entity = entities->Create();

	//	Transform* const transform = entities->GetComponent<Transform>(entity);
	//	transform->scale = vec3f(1.0f, 10.0f, 10.0f);
	//	transform->translation.x = 5.0f;
	//	transform->SetDynamic(false);

	//	MeshRender* const render = entities->AddComponent<MeshRender>(entity);
	//	render->SetActive(true);
	//	render->SetMaterial(white);
	//	render->SetModel(cube);
	//	render->SetDynamic(false);
	//}

	//{
	//	const unsigned entity = entities->Create();

	//	Transform* const transform = entities->GetComponent<Transform>(entity);
	//	transform->scale = vec3f(10.0f, 10.0f, 1.0f);
	//	transform->translation.z = -5.0f;
	//	transform->SetDynamic(false);

	//	MeshRender* const render = entities->AddComponent<MeshRender>(entity);
	//	render->SetActive(true);
	//	render->SetMaterial(white);
	//	render->SetModel(cube);
	//	render->SetDynamic(false);
	//}
}
