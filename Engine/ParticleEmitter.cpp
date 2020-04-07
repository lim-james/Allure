#include "ParticleEmitter.h"

#include <Events/EventsManager.h>

ParticleEmitter::ParticleEmitter()
	: age(0.f)
	, duration(-1.f)
	, spawnInterval(0.f)
	, spawnTimer(0.f)
	, burstAmount(1)
	, loop(false)
	, lifetime(1.f)
	, lifetimeRange(0.f)
	, angle(0.f)
	, angleRange(0.f)
	, speed(1.f)
	, speedRange(0.f)
	, offset(0.f)
	, positionRange(0.f)
	, gravity(0.f)
	, accelRad(0.f)
	, accelRadRange(0.f)
	, startSize(1.f)
	, startSizeRange(0.f)
	, endSize(1.f)
	, endSizeRange(0.f)
	, texture(0)
	, startColor(1.f)
	, startColorRange(0.f)
	, endColor(1.f)
	, endColorRange(0.f) {}

void ParticleEmitter::Initialize() {
	age = 0.f;
	duration = -1.f;

	spawnInterval = 0.f;
	spawnTimer = 0.f;
	burstAmount = 1;

	loop = false;
	lifetime = 1.f;
	lifetimeRange = 0.f;

	angle = vec3f(0.f);
	angleRange = vec3f(0.f);

	speed = 1.f;
	speedRange = 0.f;

	offset = vec3f(0.f);
	positionRange = vec3f(0.f);

	gravity = vec3f(0.f);

	accelRad = 0.f;
	accelRadRange = 0.f;

	startSize = vec3f(1.f);
	startSizeRange = vec3f(0.f);

	endSize = vec3f(1.f);
	endSizeRange = vec3f(0.f);

	texture = 0;

	startColor = vec4f(1.f);
	startColorRange = vec4f(0.f);

	endColor = vec4f(1.f);
	endColorRange = vec4f(0.f);
}

Component * ParticleEmitter::Clone() const {
	return new ParticleEmitter(*this);
}

void ParticleEmitter::SetActive(bool const& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("EMITTER_ACTIVE", new Events::AnyType<ParticleEmitter*>(this));
}

void ParticleEmitter::Play() {
	age = 0.f;
}
