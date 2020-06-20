#include "EnemyLife.h"

#include "Layers.h"
#include "Text.h"
#include "ScoreEvent.h"

#include <Events/EventsManager.h>

void EnemyLife::OnCollisionEnter(unsigned target) {
	switch (entities->GetLayer(target)) {
	case EFFECT_AREA:
	case BULLET:
		//Hit(target, false);
		break;
	case BONUS_BULLET:
		//Hit(target, true);
		break;
	default:
		break;
	}
}

void EnemyLife::Kill() {
	entities->Destroy(entity);
}

bool EnemyLife::IsStunned() const {
	return stunBt > 0.f;
}

void EnemyLife::Awake() {
	state = GetComponent<StateContainer>();
	render = GetComponent<SpriteRender>();
	physics = GetComponent<Physics>();
}

void EnemyLife::Start() {
	flashDelay = 0.1f;
	stunDelay = 0.5f;
	flashBt = stunBt = 0.f;
}

void EnemyLife::Update() {
	if (flashBt > 0.f) {
		if (!hasFroze && flashBt < flashDelay) {	
			EventsManager::Get()->Trigger("FREEZE", new Events::AnyType<float>(0.05f));
			hasFroze = true;
		}

		flashBt -= time->dt;
		if (flashBt <= 0.f) {
			render->tint.rgb = 1.f;
		} else {
			render->tint.rgb = 100.f;
		}
	} else {
		if (stunBt > 0.f) {
			stunBt -= time->dt;
			//render->tint.a = render->tint.a > 0.5f ? 0.f : 1.f;

			//if (stunBt <= 0.f) {
			//	render->tint.a = 1.f;
			//}
		}
	}
}

void EnemyLife::Hit(unsigned const& target, bool const& bonus) {
	Physics* const tPhysics = entities->GetComponent<Physics>(target);
	physics->velocity = 0.f;
	physics->AddForce(tPhysics->velocity * 25.f);

	flashBt = flashDelay;
	stunBt = stunDelay;
	hasFroze = false;

	if (shield > 0) {
		--shield;
		return;
	}

	if (--health <= 0) {
		state->queuedState = "DEAD";

		EventsManager::Get()->Trigger("SCORE", new Events::Score(
			points,
			transform->GetWorldTranslation()
		));

		if (bonus)
			EventsManager::Get()->Trigger("BUILD_MULTIPLIER");
	}
}
