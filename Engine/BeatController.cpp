#include "BeatController.h"

void BeatController::SetTempo(unsigned const & tempo) {
	delay = 60.f / static_cast<float>(tempo);
}

void BeatController::Hit() {
	if (threshold >= bt) {
		Debug::Warn << "ON TEMPO\n";
		material->et = 0.0f;
	} else {
		Debug::Warn << "TOO EARLY\n";
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
		Debug::Warn << "MISSED\n";
	}

}
