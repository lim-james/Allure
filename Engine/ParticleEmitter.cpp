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

	angle.Set(0.f);
	angleRange.Set(0.f);

	speed = 1.f;
	speedRange = 0.f;

	offset.Set(0.f);
	positionRange.Set(0.f);

	gravity.Set(0.f);

	accelRad = 0.f;
	accelRadRange = 0.f;

	startSize.Set(1.f);
	startSizeRange.Set(0.f);

	endSize.Set(1.f);
	endSizeRange.Set(0.f);

	texture = 0;

	startColor.Set(1.f);
	startColorRange.Set(0.f);

	endColor.Set(1.f);
	endColorRange.Set(0.f);
}

void ParticleEmitter::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("EMITTER_ACTIVE", new Events::AnyType<ParticleEmitter*>(this));
}

void ParticleEmitter::Play(const std::function<void()>& _completion) {
	age = 0.f;
	completion = _completion;
}
