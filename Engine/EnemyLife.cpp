#include "EnemyLife.h"

#include "Layers.h"
#include "Text.h"
#include "ScoreEvent.h"

#include <Events/EventsManager.h>

void EnemyLife::OnCollisionEnter(unsigned target) {
	if (entities->GetLayer(target) == BULLET) {
		EventsManager::Get()->Trigger("SCORE", new Events::Score(
			points,
			transform->GetWorldTranslation()
		));

		entities->Destroy(target);
		entities->Destroy(entity);
	} else if (entities->GetLayer(target) == BONUS_BULLET) {
		EventsManager::Get()->Trigger("SCORE", new Events::Score(
			points,
			transform->GetWorldTranslation()
		));

		EventsManager::Get()->Trigger("BUILD_MULTIPLIER");

		entities->Destroy(target);
		entities->Destroy(entity);
	}
}
