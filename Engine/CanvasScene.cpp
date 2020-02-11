#include "CanvasScene.h"

#include "Camera.h"

void CanvasScene::Awake() {
	Scene::Awake();

	entities->GetComponent<Camera>(mainCamera)->clearColor.Set(1.f);
}
