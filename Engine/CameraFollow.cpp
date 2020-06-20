#include "CameraFollow.h"

void CameraFollow::Update() {
	const vec3f playerPosition = player->GetWorldTranslation();

	vec3f displacement = crosshair->GetWorldTranslation() - playerPosition;
	const float d = Math::Length(displacement);
	if (d > 0.f) displacement *= min(d, jutDistance) / d;

	const vec3f position = playerPosition + displacement + offset;

	if (stick) {
		transform->SetLocalTranslation(position + offset);
	} else {
		vec3f translation = transform->GetLocalTranslation();
		const vec3f delta = position - translation;
		translation += delta * time->dt * speed;
		transform->SetLocalTranslation(translation);
	}
}
