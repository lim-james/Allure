#include "EnemyTarget.h"

#include <Events/EventsManager.h>

void EnemyTarget::Chase() {
	vec3f diff = player->GetWorldTranslation() - transform->GetWorldTranslation();
	const float len = Math::Length(diff);

	diff /= len;

	const float mag = magnitude * 10.f;

	if (len < minRadius) {
		physics->AddForce(-diff * speed * mag);
	} else {
		physics->AddForce(diff * speed * mag);
	}
}

void EnemyTarget::Awake() {
	physics = GetComponent<Physics>();
	EventsManager::Get()->Subscribe("BEAT_VALUE", &EnemyTarget::EventHandler, this);
}

void EnemyTarget::Update() {
	const vec2f diff = player->GetWorldTranslation() - transform->GetWorldTranslation();

	const float direction = diff.x;
	transform->rotation.y = 90.f - 90.f * direction / fabs(direction);
}

void EnemyTarget::EventHandler(Events::Event * event) {
	magnitude = static_cast<Events::AnyType<float>*>(event)->data;
}
