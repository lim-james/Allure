#include "BeatController.h"

#include "Text.h"

#include <Events/EventsManager.h>

void BeatController::SetTempo(unsigned const & tempo) {
	delay = 60.f / static_cast<float>(tempo);
}

void BeatController::Awake() {
	EventsManager::Get()->Subscribe("HIT_BEAT", &BeatController::HitHandler, this);
}

void BeatController::Start() {
	bt = threshold;
	isHit = false;
	endCycle = false;
}

void BeatController::Update() {
	bt -= time->dt;

	background->interval = delay;
	background->bt = bt;
	background->threshold = threshold;
	background->playerPosition = transform->GetWorldTranslation();

	if (bt < 0.f) {
		bt = delay + bt;
		endCycle = true;
		EventsManager::Get()->Trigger("BEAT");
	}

	if (endCycle && threshold < delay - bt) {
		if (!isHit) {
			Transform* const indicator = indicatorPrefab->Create();
			indicator->translation = transform->GetWorldTranslation() + vec3f(0.f, 3.f, 0.f);
		
			Text* const text = entities->GetComponent<Text>(indicator->entity);
			text->text = "MISSED";
			EventsManager::Get()->Trigger("RESET_MULTIPLIER");
		}
		endCycle = false;
		isHit = false;
	}

}

void BeatController::HitHandler(Events::Event * event) {
	bool * const state = static_cast<Events::AnyType<bool*>*>(event)->data;
	if (!isHit && (threshold >= bt || threshold >= delay - bt)) {
		background->et = 0.f;
		isHit = true;
		*state = true;
	} else {
		//Transform* const indicator = indicatorPrefab->Create();
		//indicator->translation = transform->GetWorldTranslation() + vec3f(0.f, 3.f, 0.f);

		//Text* const text = entities->GetComponent<Text>(indicator->entity);
		//text->text = "TOO EARLY";
		*state = false;
		EventsManager::Get()->Trigger("RESET_MULTIPLIER");
	}

}
