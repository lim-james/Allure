#include "Orbit.h"

#include <Math/Math.hpp>

void Orbit::Awake() {
	t = 0;
}

void Orbit::Update() {
	t += time->dt;

	const vec2f d = vec2f(sin(t), cos(t)) * radius;
	transform->translation.xy = orbit->translation.xy + d;
}
