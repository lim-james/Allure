#include "LlamaScene.h"

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
#include "LoadTexture.h"
#include "LoadOBJ.h"
#include "Layers.h"

void LlamaScene::Create() {
	Scene::Create();

	Material::MeshDefault* white = new Material::MeshDefault;
	white->albedo = vec4f(1.f);
	white->ao = 0.1f;

	Model* const cube = Load::OBJ("Files/Models/cube.obj");

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
		transform->rotation = vec3f(50.f, -30.f, 0.f);
		transform->UpdateAxes();

		Light* const light = entities->AddComponent<Light>(entity);
		light->SetActive(true);
		light->color = vec4f(253.f / 255.f, 251.f / 255.f, 211.f / 255.f, 1.0f);
		light->type = LIGHT_DIRECTIONAL;
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->scale = vec3f(10.0f, 1.0f, 10.0f);
		transform->translation.y = -5.f;
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(white);
		render->SetModel(cube);
		render->SetDynamic(false);
	}
}
