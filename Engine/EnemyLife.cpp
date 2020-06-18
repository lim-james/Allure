#include "EnemyLife.h"

#include "Layers.h"
#include "Text.h"
#include "ScoreEvent.h"

#include <Events/EventsManager.h>

void EnemyLife::OnCollisionEnter(unsigned target) {
	switch (entities->GetLayer(target)) {
	case EFFECT_AREA:
	case BULLET:
		Hit(target, false);
		break;
	case BONUS_BULLET:
		Hit(target, true);
		break;
	default:
		break;
	}
}

void EnemyLife::Kill() {
	entities->Destroy(entity);
}

void EnemyLife::Awake() {
	state = GetComponent<StateContainer>();
	render = GetComponent<SpriteRender>();
	physics = GetComponent<Physics>();
}

void EnemyLife::Start() {
	flashBt = 0.1f;
	bt = 0.f;
}

void EnemyLife::Update() {
	if (bt > 0.f) {
		if (!hasFroze && bt < flashBt) {	
			EventsManager::Get()->Trigger("FREEZE", new Events::AnyType<float>(0.05f));
			hasFroze = true;
		}

		bt -= time->dt;
		if (bt <= 0.f) {
			render->tint.rgb = 1.f;
		} else {
			render->tint.rgb = 100.f;
		}
	}
}

void EnemyLife::Hit(unsigned const& target, bool const& bonus) {
	Physics* const tPhysics = entities->GetComponent<Physics>(target);
	physics->velocity = 0.f;
	physics->AddForce(tPhysics->velocity * 25.f);

	bt = flashBt;
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
