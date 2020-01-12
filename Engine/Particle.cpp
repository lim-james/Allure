#include "Particle.h"

#include <Events/EventsManager.h>

Particle::Particle()
	: age(0.f)
	, lifetime(0.f)
	, velocity(0.f)
	, startSize(1.f)
	, endSize(1.f)
	, startColor(1.f)
	, endColor(1.f) {}

void Particle::Initialize() {
	age = 0.f;
	lifetime = 0.f;

	velocity.Set(0.f);

	startSize.Set(1.f);
	endSize.Set(1.f);

	startColor.Set(1.f);
	endColor.Set(1.f);
}

