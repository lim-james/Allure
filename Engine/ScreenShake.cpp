#include "ScreenShake.h"

#include <Events/EventsManager.h>

void ScreenShake::Awake() {
	k = Math::PI * 6.f;
	EventsManager::Get()->Subscribe("SCREEN_SHAKE", &ScreenShake::ShakeHandler, this);
}

void ScreenShake::Update() {
	if (et < duration) {
		const float x = et * k / duration;
		const vec2f d = displacement * sin(x) / (x * 0.5f);
		transform->translation.xy += d;
		et += time->dt;
	}
}

void ScreenShake::ShakeHandler(Events::Event * event) {
	const vec2f dir = static_cast<Events::AnyType<vec2f>*>(event)->data;
	et = time->dt;
	displacement = Math::Normalized(dir) * magnitude;
}
