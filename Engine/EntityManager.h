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
		std::map<unsigned, std::vector<Component*>> components;
	};

	std::vector<Component*> componentPool;

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
	ComponentType* AddComponent(unsigned const& id, bool const& unique = true);
	template<typename ComponentType>
	bool HasComponent(unsigned const& id);
	template<typename ComponentType>
	ComponentType* GetComponent(unsigned const& id);
	template<typename ComponentType>
	std::vector<ComponentType*> GetComponents(unsigned const& id);

	unsigned PoolCount() const;

private:
	
	void Expand();

	void DestroyEntity(unsigned const& id);

};

#include "EntityManager.inl"

#endif

