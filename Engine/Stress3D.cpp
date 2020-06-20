#include "Stress3D.h"

#include "Camera.h"
#include "Light.h"
#include "Text.h"
#include "MeshRender.h"
#include "VoxelRender.h"
#include "EditorCamera.h"
#include "FPSCounter.h"

#include "LoadFNT.h"
#include "LoadModel.h"
#include "Layers.h"

#include <Math/Random.hpp>

void Stress3D::Create() {
	Scene::Create();

	Font* const courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	Mesh* const cube = Load::OBJ("Files/Models/cube.obj")->meshes[0];

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->clearColor = vec4f(0.f, 0.f, 0.f, 1.f);
	camera->cullingMask -= UI;

	{
		EditorCamera* const editorCamera = entities->AddComponent<EditorCamera>(mainCamera);
		editorCamera->SetActive(true);
	}

	// UI Camera
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, 10.f));

		Camera* const uiCamera = entities->AddComponent<Camera>(entity);
		uiCamera->SetActive(true);
		uiCamera->SetSize(10.f);
		uiCamera->SetDepth(1);
		uiCamera->shouldClear = false;
		uiCamera->projection = ORTHOGRAPHIC;
		uiCamera->cullingMask = UI;
	}

	// directional light
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalRotation(vec3f(50.f, -30.f, 0.f));

		Light* const light = entities->AddComponent<Light>(entity);
		light->SetActive(true);
		light->color = vec4f(253.f / 255.f, 251.f / 255.f, 211.f / 255.f, 1.0f);
		light->type = LIGHT_DIRECTIONAL;
		light->SetCastShadows(false);
	}

	// FPS counter
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(-14.f, 8.f, 0.f));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(courierNew);

		FPSCounter* const fps = entities->AddComponent<FPSCounter>(entity);
		fps->SetActive(true);
	}

	float size = 20;
	for (float x = -size; x <= size; ++x) {
		for (float y = -size; y <= size; ++y) {
			for (float z = -size; z <= size; ++z) {
				const unsigned entity = entities->Create();

				Transform* const transform = entities->GetComponent<Transform>(entity);
				transform->SetLocalTranslation(vec3f(x * 1.5f, y * 1.5f, z * 1.5f));
				transform->SetDynamic(false);

				VoxelRender* const render = entities->AddComponent<VoxelRender>(entity);
				//MeshRender* const render = entities->AddComponent<MeshRender>(entity);
				render->SetActive(true);
				render->tint.rgb = vec3f(Math::RandValue(), Math::RandValue(), Math::RandValue());
				//render->SetMesh(cube);
				render->SetDynamic(false);
			}
		}
	}
}
