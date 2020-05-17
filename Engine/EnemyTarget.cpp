#include "EnemyTarget.h"

void EnemyTarget::Chase() {
	vec3f diff = player->GetWorldTranslation() - transform->GetWorldTranslation();
	Math::Normalize(diff);
	physics->AddForce(diff * speed);
}

void EnemyTarget::Awake() {
	physics = GetComponent<Physics>();
}
