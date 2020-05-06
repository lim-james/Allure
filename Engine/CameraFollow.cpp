#include "CameraFollow.h"

void CameraFollow::Update() {
	if (stick) {
		transform->translation = target->translation + offset;
	} else {
		const vec3f delta = target->translation + offset - transform->translation;
		transform->translation += delta * time->dt * speed;
	}
}
