#include "Orbit.h"

#include <Math/Math.hpp>

void Orbit::Awake() {
	t = 0;
}

void Orbit::Update() {
	t += time->dt;

	const vec3f d = vec3f(sin(t) * 0.5f, cos(t), sin(t)) * radius;
	transform->SetLocalTranslation(orbit->GetWorldTranslation() + d);
}
