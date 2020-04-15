#include "SampleScene.h"

// component
#include "Camera.h"
#include "FPSCamera.h"
// Utils
#include "Layers.h"

void SampleScene::Awake() {
	Scene::Awake();
}

void SampleScene::Create() {
	Scene::Create();

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->cullingMask -= UI;
}
