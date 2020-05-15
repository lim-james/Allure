#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Component.h"

#include <Containers/QuadTree.hpp>
#include <Events/Event.h>

#include <MACROS.h>
#include <Logger/Logger.h>

#include <typeindex>
#include <vector>
#include <map>

class EntityManager {

	struct Entity {
		unsigned layer;
		std::map<unsigned, Component*> components;
	};

	std::vector<Entity> entities;

	std::vector<unsigned> used;
	std::vector<unsigned> unused;

	std::vector<unsigned> destroyQueue;

public:

	~EntityManager();

	unsigned Create();

	void Destroy(unsigned const& id);
	void Update();

	void SetLayer(unsigned const& id, unsigned const& layer);
	unsigned const& GetLayer(unsigned const& id);

	template<typename ComponentType>
	ComponentType* AddComponent(unsigned const& id);
	template<typename ComponentType>
	bool HasComponent(unsigned const& id);
	template<typename ComponentType>
	ComponentType* GetComponent(unsigned const& id);

	unsigned PoolCount() const;

private:
	
	void Expand();

};

template<typename ComponentType>
ComponentType * EntityManager::AddComponent(unsigned const& id) {
	if (id >= entities.size())
		return nullptr;

	const unsigned hash = hashof(ComponentType);
	auto& components = entities[id].components;

	if (HasComponent<ComponentType>(id))
		return static_cast<ComponentType*>(components[hash]);

	ComponentType* c = new ComponentType;
	c->entity = id;
	c->Initialize();
	components[hash] = c;

	return c;
}

template<typename ComponentType>
bool EntityManager::HasComponent(unsigned const& id) {
	if (id >= entities.size())
		return false;

	return entities[id].components[hashof(ComponentType)] != nullptr;
}

template<typename ComponentType>
ComponentType* EntityManager::GetComponent(unsigned const& id) {
	if (id >= entities.size())
		return nullptr;

	auto& components = entities[id].components;
	Component* const c = components[hashof(ComponentType)];
	return c ? static_cast<ComponentType*>(c) : nullptr;
}

#endif

