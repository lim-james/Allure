#include "SampleScene.h"

// component
#include "Camera.h"
#include "EditorCamera.h"
#include "SpriteRender.h"
#include "MeshRender.h"
// materials
#include "UnlitDefaultMaterial.h"
// Utils
#include "LoadOBJ.h"
#include "Layers.h"

void SampleScene::Awake() {
	Scene::Awake();
}

void SampleScene::Create() {
	Scene::Create();

	Material::UnlitDefault* opaque = new Material::UnlitDefault;
	opaque->alphaClipping = 0.01f;

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->cullingMask -= UI;

	{
		EditorCamera* const editorCamera = entities->AddComponent<EditorCamera>(mainCamera);
		editorCamera->SetActive(true);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->scale.xy = 5.0f;
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMaterial(opaque);
		render->SetModel(Load::OBJ("Files/Models/cube.obj"));
		render->SetDynamic(false);
	}
}
