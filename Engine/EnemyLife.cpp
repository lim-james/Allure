#include "EnemyLife.h"

#include "Layers.h"
#include "Text.h"
#include "ScoreEvent.h"

#include <Events/EventsManager.h>

void EnemyLife::OnCollisionEnter(unsigned target) {
	switch (entities->GetLayer(target)) {
	case EFFECT_AREA:
		Hit(false);
		break;
	case BULLET:
		entities->Destroy(target);
		Hit(false);
		break;
	case BONUS_BULLET:
		entities->Destroy(target);
		Hit(true);
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
}

void EnemyLife::Start() {
	flashBt = 0.1f;
	bt = 0.f;
}

void EnemyLife::Update() {
	if (bt > 0.f) {
		bt -= time->dt;
		if (bt < 0.f) {
			render->tint.rgb = 1.f;
		} else {
			render->tint.rgb = 100.f;
		}
	}
}

void EnemyLife::Hit(bool const& bonus) {
	bt = flashBt;

	if (--lives <= 0) {
		state->queuedState = "DEAD";

		EventsManager::Get()->Trigger("SCORE", new Events::Score(
			points,
			transform->GetWorldTranslation()
		));

		if (bonus)
			EventsManager::Get()->Trigger("BUILD_MULTIPLIER");
	}
}
