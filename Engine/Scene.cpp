#include "Scene.h"

#include "TransformSystem.h"
#include "RenderSystem.h"

#include <Math/Vectors.h>
#include <Events/EventsManager.h>

Scene::Scene() {
	entities = new EntityManager;
	systems = new SystemManager(entities);
	scripts = new ScriptSystem(entities);
}

Scene::~Scene() {
	delete entities;
	delete systems;
}

void Scene::Awake() {
	systems->Subscribe<TransformSystem>(0);
	systems->Subscribe<RenderSystem>(0);

	scripts->Awake();
}

void Scene::Create() {
	mainCamera = entities->Create();
	entities->GetComponent<Transform>(mainCamera)->translation.z = 1.f;
	Camera * camera = entities->AddComponent<Camera>(mainCamera);
	camera->SetActive(true);
	camera->clearColor.Set(0.f);
}

void Scene::Enter() {
	Events::EventsManager::GetInstance()->SubscribeContext(this);
	systems->Start();
	scripts->Start();
}

void Scene::FixedUpdate(const float& dt) {
	scripts->FixedUpdate();
}

void Scene::Update(const float& dt) {
	systems->Update(dt);
	scripts->Update(dt);
}

void Scene::Exit() {
	scripts->Stop();
	systems->Stop();
	Events::EventsManager::GetInstance()->UnsubscribeContext(this);
}

void Scene::Destroy() { 
	scripts->Destroy();
}

void Scene::PrepareForSegue(Scene * destination) { }
