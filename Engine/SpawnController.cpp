#include "SpawnController.h"

#include "Physics.h"

#include <Math/Random.hpp>

void SpawnController::SetMapPath(std::string const & filepath) {
	map.Load(filepath);
	map.Initialize();
}

void SpawnController::SetOffset(float const & t) {
	offset = t;
	map.SetOffset(travelTime + offset);
}

void SpawnController::SetOuterRadius(float const & value) {
	outerRadius = value;
	UpdateSpeed();
}

void SpawnController::SetInnerRadius(float const & value) {
	innerRadius = value;
	UpdateSpeed();
}

void SpawnController::SetTravelTime(float const & t) {
	travelTime = t;
	map.SetOffset(travelTime + offset);
	UpdateSpeed();
}

void SpawnController::Start() {
	source->Play();
	angle = Math::RandMinMax(0.f, Math::PI * 2.f);
}

void SpawnController::Update() {
	if (source->IsPaused()) return;
	
	const float dt = time->dt * source->speed;
	map.Update(dt);
	angle += dt * rotationSpeed;

	if (map.Check()) {
		const vec3f direction = vec3f(cos(angle), sin(angle), 0.f);
		Transform* const bulletTransform = bulletPrefab->CreateAt(direction * outerRadius);

		Physics* const physics = entities->GetComponent<Physics>(bulletTransform->entity);
		physics->velocity = direction * -speed;
	}
}

void SpawnController::UpdateSpeed() {
	if (travelTime == 0.f) {
		speed = 1.f;
		return;
	}

	speed = (outerRadius - innerRadius) / travelTime;
}
