#include "FPSCounter.h"

void FPSCounter::Awake() {
	text = GetComponent<Text>();
}

void FPSCounter::Update() {
	text->text = std::to_string((int)(1.f / time->dt));
}
