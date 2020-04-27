#include "LowPolyScene.h"	

// components
#include "Camera.h"
#include "EditorCamera.h"
#include "Light.h"
#include "MeshRender.h"
// material
#include "MeshDefaultMaterial.h"
// utils
#include "Layers.h"
#include "LoadModel.h"

void LowPolyScene::Create() {
	Scene::Create();

	Model* const model = Load::Meshes("Files/Models/Terrain_2.fbx");
	Model* const tree1 = Load::Meshes("Files/Models/Environment/Tree_1.fbx");
	Model* const tree2 = Load::Meshes("Files/Models/Environment/Tree_2.fbx");
	Model* const tree3 = Load::Meshes("Files/Models/Environment/Tree_3.fbx");

	Material::MeshDefault* const grass = new Material::MeshDefault;
	grass->albedo = vec4f(120.f / 255.f, 224.f / 255.f, 143.f / 255.f, 1.f);

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->farPlane = 1000.f;
	camera->clearColor = vec4f(130.f / 255.f, 204.f / 255.f, 221.f / 255.f, 1.f);
	camera->cullingMask -= UI;

	{
		EditorCamera* const editorCamera = entities->AddComponent<EditorCamera>(mainCamera);
		editorCamera->SetActive(true);
		editorCamera->speed = 100.f;
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
		light->intensity = 2.f;
	}

	// tree
	for (Mesh* const mesh : tree2->meshes) {
		const unsigned entity = entities->Create();
		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->scale = 0.1f;
		transform->SetDynamic(false);

		MeshRender* const render = entities->AddComponent<MeshRender>(entity);
		render->SetActive(true);
		render->SetMesh(mesh);
		render->SetMaterial(grass);
		render->SetDynamic(false);
	}
}
