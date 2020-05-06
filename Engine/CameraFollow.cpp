#include "CameraFollow.h"

void CameraFollow::Update() {
	vec3f displacement = crosshair->translation - player->translation;
	const float d = Math::Length(displacement);
	if (d > 0.f) displacement *= min(d, jutDistance) / d;

	const vec3f position = player->translation + displacement + offset;

	if (stick) {
		transform->translation = position + offset;
	} else {
		const vec3f delta = position - transform->translation;
		transform->translation += delta * time->dt * speed;
	}
}
