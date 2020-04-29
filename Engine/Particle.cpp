#include "Particle.h"

#include <Events/EventsManager.h>

Particle::Particle()
	: age(0.f)
	, lifetime(0.f)
	, velocity(0.f)
	, angularVelocity(0.f)
	, startSize(1.f)
	, endSize(1.f)
	, startColor(1.f)
	, endColor(1.f) {}

void Particle::Initialize() {
	age = 0.f;
	lifetime = 0.f;

	velocity = vec3f(0.f);
	angularVelocity = vec3f(0.f);

	startSize = vec3f(1.f);
	endSize = vec3f(1.f);

	startColor = vec3f(1.f);
	endColor = vec3f(1.f);
}

Component * Particle::Clone() const {
	return new Particle(*this);
}

