#include "Physics.h"

#include <Events/EventsManager.h>

Physics::Physics()
	: useGravity(true)
	, mass(1.f)
	, inverseMass(1.f)
	, drag(0.f) 
	, force(0.f) 
	, velocity(0.f) {
}

void Physics::Initialize() {
	useGravity = true;

	mass = 1.f;
	inverseMass = 1.f;

	drag = 0.f;
	force = vec3f(0.f);
	velocity = vec3f(0.f);
}

Component * Physics::Clone() const {
	return new Physics(*this);
}

void Physics::SetActive(bool const& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("PHYSICS_ACTIVE", new Events::AnyType<Physics*>(this));
}

void Physics::SetMass(float const& _mass) {
	mass = _mass;
	inverseMass = 1.f / mass;
}

float Physics::GetMass() const {
	return mass;
}

float Physics::GetInverseMass() const {
	return inverseMass;
}
