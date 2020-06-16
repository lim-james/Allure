#include "Prefab.h"

void Prefab::Initialize(EntityManager * const manager) {
	entities = manager;
}

Transform * Prefab::CreateIn(Transform * const parent) {
	Transform* const self = Create();
	self->SetParent(parent);
	return self;
}

Transform * Prefab::CreateIn(Transform * const parent, vec3f const & position) {
	Transform* const self = Create();
	self->SetParent(parent);
	self->translation = position;
	return self;
}
