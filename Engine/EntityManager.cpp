#include "EntityManager.h"

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
		for (auto& component : entity) {
			delete component.second;
		}
		entity.clear();
	}
	entities.clear();
}

void EntityManager::Destroy(const unsigned & id) {
	if (Helpers::Remove(used, id)) {
		for (auto& pair : entities[id]) {
			if (pair.second) {
				pair.second->Initialize();
				pair.second->SetActive(false);
			}
		}

		unused.push_back(id);
	}
}

void EntityManager::Expand() {
	const unsigned id = entities.size();
	entities.push_back({});
	unused.push_back(id);
	AddComponent<Transform>(id);
}

