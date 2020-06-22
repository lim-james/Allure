#include "EntityManager.h"

#include "Layers.h"
#include "Transform.h"
#include <Helpers/VectorHelpers.h>

unsigned EntityManager::Create() {
	if (unused.empty()) Expand();

	const unsigned id = unused.front();
	unused.erase(unused.begin());
	used.push_back(id);

	GetComponent<Transform>(id)->SetActive(true);

	return id;
}

EntityManager::~EntityManager() {
	for (Component* c : componentPool)
		delete c;
}

void EntityManager::Destroy(unsigned const& id) {
	destroyQueue.push_back(id);
}

void EntityManager::Update() {
	for (unsigned const& id : destroyQueue)
		DestroyEntity(id);
	destroyQueue.clear();
}

void EntityManager::SetLayer(unsigned const & id, unsigned const & layer) {
	entities[id].layer = layer;
}

unsigned const & EntityManager::GetLayer(unsigned const & id) {
	return entities[id].layer;
}

unsigned EntityManager::PoolCount() const {
	return entities.size();
}

void EntityManager::Expand() {
	Entity e;
	e.layer = DEFAULT;

	const unsigned id = entities.size();
	entities.push_back(e);
	unused.push_back(id);
	AddComponent<Transform>(id);
}

void EntityManager::DestroyEntity(unsigned const & id) {
	if (Helpers::Remove(used, id)) {
		for (Transform* const transform : GetComponent<Transform>(id)->GetChildren()) {
			DestroyEntity(transform->entity);
		}

		for (auto& pair : entities[id].components) {
			for (Component* c : pair.second) {
				c->Initialize();
				c->SetActive(false);
			}
		}

		unused.push_back(id);
	}
}

