#include "ScreenShake.h"

#include <Events/EventsManager.h>

void ScreenShake::Awake() {
	k = Math::PI * 6.f;
	EventsManager::Get()->Subscribe("SCREEN_SHAKE", &ScreenShake::ShakeHandler, this);
}

void ScreenShake::Update() {
	if (et < duration) {
		if (et != 0.f) {
			const float x = et * k / duration;
			const vec2f d = displacement * sin(x) / (x * 0.5f);

			vec3f translation = transform->GetLocalTranslation();
			translation.xy += d;

			transform->SetLocalTranslation(translation);
		}
		et += time->dt;
	}
}

void ScreenShake::ShakeHandler(Events::Event * event) {
	const vec2f dir = static_cast<Events::AnyType<vec2f>*>(event)->data;
	et = time->dt;
	displacement = Math::Normalized(dir) * magnitude;
}
