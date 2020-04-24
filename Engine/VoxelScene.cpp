#include "VoxelScene.h"

// component
#include "Camera.h"
#include "Light.h"
#include "EditorCamera.h"
#include "VoxelRenderer.h"
// Utils
#include "Layers.h"

#include <Math/Random.hpp>

void VoxelScene::Create() {
	Scene::Create();

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->clearColor = 0.5f;
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

	const float size = 10.f;

	for (float x = -size; x <= size; ++x) { 
		for (float z = -size; z <= size; ++z) {
			const unsigned entity = entities->Create();

			Transform* const transform = entities->GetComponent<Transform>(entity);
			transform->translation = vec3f(x, -1.f, z);
			transform->SetDynamic(false);

			VoxelRender* const render = entities->AddComponent<VoxelRender>(entity);
			render->SetActive(true);
			render->tint = vec4f(Math::RandValue(), Math::RandValue(), Math::RandValue(), 1.f);
			render->SetDynamic(false);
		}
	}
}
