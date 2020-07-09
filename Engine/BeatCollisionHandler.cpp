#include "BeatCollisionHandler.h"

void BeatCollisionHandler::Awake() {
	container = GetComponent<StateContainer>();
}

void BeatCollisionHandler::OnHit(unsigned target) {
	container->queuedState = "BEAT_DESTROYED";
}
