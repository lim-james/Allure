#include "PhysicsSystem.h"

#include "Transform.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>
#include <MACROS.h>

PhysicsSystem::~PhysicsSystem() {
	components.clear();
}

void PhysicsSystem::Initialize() {
	gravity = vec3f(0.f, -9.8f * 5.f, 0.f);

	Events::EventsManager::GetInstance()->Subscribe("PHYSICS_ACTIVE", &PhysicsSystem::ActiveHandler, this);
}

void PhysicsSystem::Update(float const& dt) {
	for (auto& c : components) {
		auto t = entities->GetComponent<Transform>(c->entity);

		if (c->useGravity)
			c->velocity += gravity * dt;

		c->velocity += c->force * c->GetInverseMass() * dt;
		t->translation += c->velocity * dt;
	}
}

void PhysicsSystem::ActiveHandler(Events::Event* event) {
	const auto c = static_cast<Events::AnyType<Physics*>*>(event)->data;
	const auto position = vfind(components, c);

	if (c->IsActive()) {
		Helpers::Insert(components, c);
	} else {
		Helpers::Remove(components, c);
	}
}
