#include "BeatController.h"

#include "Text.h"

void BeatController::SetTempo(unsigned const & tempo) {
	delay = 60.f / static_cast<float>(tempo);
}

void BeatController::Hit() {
	if (threshold >= bt) {
		material->et = 0.0f;
	} else {
		{
			Transform* const indicator = indicatorPrefab->Create();
			indicator->translation = transform->GetWorldTranslation() + vec3f(0.f, 3.f, 0.f);

			Text* const text = entities->GetComponent<Text>(indicator->entity);
			text->text = "TOO EARLY";
		}
	}

	bt = delay;
}

void BeatController::Start() {
	bt = delay;
}

void BeatController::Update() {
	bt -= time->dt;

	material->interval = delay;
	material->bt = bt;
	material->threshold = threshold;
	material->playerPosition = transform->GetWorldTranslation();

	if (bt < 0.f) {
		bt = delay;
		{
			Transform* const indicator = indicatorPrefab->Create();
			indicator->translation = transform->GetWorldTranslation() + vec3f(0.f, 3.f, 0.f);
		
			Text* const text = entities->GetComponent<Text>(indicator->entity);
			text->text = "MISSED";
		}
	}

}
