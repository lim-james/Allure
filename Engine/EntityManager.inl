#include "EntityManager.h"

template<typename ComponentType>
ComponentType * EntityManager::AddComponent(unsigned const& id, bool const& unique) {
	if (id >= entities.size())
		return nullptr;

	const unsigned hash = hashof(ComponentType);
	auto& components = entities[id].components;

	if (unique && HasComponent<ComponentType>(id))
		return static_cast<ComponentType*>(components[hash][0]);

	ComponentType* c = new ComponentType;
	c->entity = id;
	c->Initialize();
	componentPool.push_back(c);
	components[hash].push_back(c);

	const unsigned baseHash = hashof(ComponentType::base_type);
	if (baseHash != hash)  components[baseHash].push_back(c);

	return c;
}

template<typename ComponentType>
bool EntityManager::HasComponent(unsigned const& id) {
	if (id >= entities.size())
		return false;

	auto& components = entities[id].components;
	if (components.find(hashof(ComponentType)) == components.end())
		return false;

	return !components[hashof(ComponentType)].empty();
}

template<typename ComponentType>
ComponentType* EntityManager::GetComponent(unsigned const& id) {
	if (!HasComponent<ComponentType>(id))
		return nullptr;

	auto& components = entities[id].components[hashof(ComponentType)];
	return static_cast<ComponentType*>(components[0]);
}

template<typename ComponentType>
std::vector<ComponentType*> EntityManager::GetComponents(unsigned const & id) {
	if (id >= entities.size() || !HasComponent<ComponentType>(id))
		return {};

	auto& components = entities[id].components[hashof(ComponentType)];
	const unsigned size = components.size();
	
	std::vector<ComponentType*> result(size);
	for (unsigned i = 0; i < size; ++i)
		result[i] = static_cast<ComponentType*>(components[i]);

	return result;
}

template<typename ComponentType>
void EntityManager::RemoveComponents(unsigned const & id) {
	if (id >= entities.size() && !HasComponent<ComponentType>(id))
		return;

	const unsigned hash = hashof(ComponentType);
	auto& components = entities[id].components;

	components.erase(hash);

	for (auto it = components.rbegin(); it != components.rend(); ++it) {
		if (dynamic_cast<ComponentType*>(it->second[0])) {
			components.erase(it->first);
		}
	}
}

