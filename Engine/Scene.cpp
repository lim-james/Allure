#include "Scene.h"

// components
//#include "Transform.h"
//#include "StateContainer.h"
// events
//#include "EntityEvents.h"
#include "TransformSystem.h"

#include <Math/Vectors.hpp>
#include <Events/EventsManager.h>

Scene::Scene() {
	//components = new ComponentsManager;
	entities = new EntityManager;
	systems = new SystemManager(entities);
}

Scene::~Scene() {
	delete entities;
	//delete components;
	delete systems;
}

void Scene::Awake() {
	systems->Subscribe<TransformSystem>(0);
}

void Scene::Reset() {
	Events::EventsManager::GetInstance()->SubscribeContext(this);
	systems->Start();
}

void Scene::Start() {
	Console::Warn << "Start\n";
}

void Scene::FixedUpdate(const float& dt) {

}

void Scene::Update(const float& dt) {
	systems->Update(dt);
}

void Scene::Stop() {
	systems->Stop();

	Events::EventsManager::GetInstance()->UnsubscribeContext(this);
}

void Scene::Destroy() {
	//entities->Destroy();
}

void Scene::PrepareForSegue(Scene * destination) { }
