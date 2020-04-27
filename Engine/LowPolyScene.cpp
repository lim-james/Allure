#include "LowPolyScene.h"	

// components
#include "Camera.h"
#include "EditorCamera.h"
#include "Light.h"
// utils
#include "Layers.h"

void LowPolyScene::Create() {
	Scene::Create();

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->clearColor = vec4f(1.f, 0.f, 0.f, 1.f);
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

}
