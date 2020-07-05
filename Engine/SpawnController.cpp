#include "SpawnController.h"

#include "Physics.h"

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
	map.SetOffset(travelTime);
	UpdateSpeed();
}

void SpawnController::SetMapPath(std::string const & filepath) {
	map.Load(filepath);
	map.Initialize();
}

void SpawnController::Start() {
	source->Play();
}

void SpawnController::Update() {
	if (source->IsPaused()) return;
	map.Update(time->dt * source->speed);

	if (map.Check()) {
		Transform* const bulletTransform = bulletPrefab->CreateAt(vec3f(0.f, outerRadius, 0.f));

		Physics* const physics = entities->GetComponent<Physics>(bulletTransform->entity);
		physics->velocity = vec3f(0.f, -1.f, 0.f) * speed;
	}
}

void SpawnController::UpdateSpeed() {
	if (travelTime == 0.f) {
		speed = 1.f;
		return;
	}

	speed = (outerRadius - innerRadius) / travelTime;
}
