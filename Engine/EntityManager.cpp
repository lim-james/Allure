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
	for (auto& entity : entities) {
		for (auto& component : entity.components) {
			delete component.second;
		}
		entity.components.clear();
	}
	entities.clear();
}

void EntityManager::Destroy(unsigned const& id) {
	if (Helpers::Remove(used, id)) {
		for (auto& pair : entities[id].components) {
			if (pair.second) {
				pair.second->Initialize();
				pair.second->SetActive(false);
			}
		}

		unused.push_back(id);
	}
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

