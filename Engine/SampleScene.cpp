#include "SampleScene.h"

// component
#include "Camera.h"
#include "FPSCamera.h"
#include "SpriteRender.h"
#include "MeshRender.h"
// Utils
#include "LoadOBJ.h"
#include "Layers.h"

void SampleScene::Awake() {
	Scene::Awake();
}

void SampleScene::Create() {
	Scene::Create();

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->cullingMask -= UI;

	{
		FPSCamera* const fps = entities->AddComponent<FPSCamera>(mainCamera);
		fps->SetActive(true);
		fps->speed = 50.0f;
	}

	{
		//const unsigned entity = entities->Create();

		//SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		//render->SetActive(true);
	}

	for (float x = -20.f; x <= 20.f; x += 1.0f) {
		for (float y = -20.f; y <= 20.f; y += 1.0f) {
			const unsigned entity = entities->Create();

			Transform* const transform = entities->GetComponent<Transform>(entity);
			transform->translation = vec3f(x, y, 0.0f);
			transform->scale = vec3f(0.5f);
			transform->SetDynamic(false);

			MeshRender* const render = entities->AddComponent<MeshRender>(entity);
			render->SetActive(true);
			if (rand() % 3) {
				render->SetModel(Load::OBJ("Files/Models/cube.obj"));
			} else if (rand() % 2) {
				render->SetModel(Load::OBJ("Files/Models/monkey.obj"));
			} else {
				render->SetModel(Load::OBJ("Files/Models/sphere.obj"));
			}
			render->SetDynamic(false);
		}
	}
}
