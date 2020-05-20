#include "EnemyTarget.h"

void EnemyTarget::Chase() {
	vec3f diff = player->GetWorldTranslation() - transform->GetWorldTranslation();
	Math::Normalize(diff);
	physics->AddForce(diff * speed);
}

void EnemyTarget::Awake() {
	physics = GetComponent<Physics>();
}

void EnemyTarget::Update() {
	const vec2f diff = player->GetWorldTranslation() - transform->GetWorldTranslation();

	const float direction = diff.x;
	transform->rotation.y = 90.f - 90.f * direction / fabs(direction);
}
