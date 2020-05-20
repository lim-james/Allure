#include "ExpandScript.h"

void ExpandScript::Start() {
	et = 0.f;
}

void ExpandScript::Update() {
	const float t = duration - et;
	const vec3f diff = targetSize - transform->scale;

	transform->scale += diff * time->dt / t;
	et += time->dt;
}
