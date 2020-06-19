#include "EnemyTarget.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>

void EnemyTarget::Chase() {
	if (life->IsStunned()) return;

	const vec3f diff = Math::Normalized(target - transform->GetWorldTranslation().xy);
	const float mag = magnitude * 10.f;
	physics->AddForce(diff * style->speed);
}

vec3f EnemyTarget::GetTarget() const {
	return vec3f(target, 0.f);
}

void EnemyTarget::Awake() {
	life = GetComponent<EnemyLife>();
	physics = GetComponent<Physics>();
	EventsManager::Get()->Subscribe("BEAT_VALUE", &EnemyTarget::EventHandler, this);
}

void EnemyTarget::Update() {
	const vec2f position = transform->GetWorldTranslation().xy;
	const vec2f playerPosition = player->GetWorldTranslation().xy;
	const float fromPlayer = Math::Length(playerPosition - position);

	if (isNear) {
		if (fromPlayer >= farStyle.radius) {
			isNear = false;
			style = &farStyle;
		} else {
			style = &nearStyle;
		}
	} else {
		if (fromPlayer <= nearStyle.radius) {
			isNear = true;
			style = &nearStyle;
		} else {
			style = &farStyle;
		}
	}

	vec2f diff = target - position;
	const float len = Math::Length(diff);

	switch (style->type) {
	case TARGET_LOCKON:
		target = playerPosition;
		break;
	case TARGET_DASH:
		if (len < 1.f) {
			target = playerPosition;
		}
		break;
	case TARGET_RANDOM:
		if (len < 1.f) {
			target.x = Math::RandMinMax(-boundary.x, boundary.x);
			target.y = Math::RandMinMax(-boundary.y, boundary.y);
		}
		break;
	}

	const float direction = target.x - position.x;
	transform->rotation.y = 90.f - 90.f * direction / fabs(direction);
}

void EnemyTarget::EventHandler(Events::Event * event) {
	magnitude = static_cast<Events::AnyType<float>*>(event)->data;
}
