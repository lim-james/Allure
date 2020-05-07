#include "PhysicsSystem.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>

void PhysicsSystem::Initialize() {
	EventsManager::Get()->Subscribe("PHYSICS_ACTIVE", &PhysicsSystem::ActiveHandler, this);
}

void PhysicsSystem::Update(float const & dt) {
	const float step = dt * invFR;

	for (auto const& deltaPair : deltas) {
		Transform* const transform = deltaPair.first;
		Delta const& delta = deltaPair.second;

		transform->translation += delta.translation * step;
		transform->rotation += delta.rotation * step;
	}
}

void PhysicsSystem::FixedUpdate(float const & dt) {
	invFR = 1.f / dt;
	deltas.clear();

	for (Physics* const c : components) {
		Transform* const transform = entities->GetComponent<Transform>(c->entity);

		c->velocity *= 1.f - c->drag * dt;
		c->angularVelocity *= 1.f - c->angularDrag * dt;

		if (c->useGravity) c->velocity.y += GRAVITY * dt;

		c->velocity += c->netForce * c->invMass * dt;
		c->angularVelocity += c->torque * c->invMass * dt;

		c->netForce = 0.f;
		c->torque = 0.f;

		if (c->interpolate) {
			deltas[transform] = Delta{ c->velocity * dt, c->angularVelocity * dt };
		} else {
			transform->translation += c->velocity * dt;
			transform->rotation += c->angularVelocity * dt;
		}
	}
}

void PhysicsSystem::ActiveHandler(Events::Event * event) {
	Physics* const c = static_cast<Events::AnyType<Physics*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(components, c);
	} else {
		Helpers::Remove(components, c);
	}
}
