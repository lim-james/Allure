#include "Light.h"

#include "Layers.h"

#include <Events/EventsManager.h>

Light::Light() 
	: type(LIGHT_DIRECTIONAL)
	, color(1.f) 
	, intensity(1.f) 
	, indirectMultiplier(1.f) 
	, castShadow(true)
	, strength(1.f)
	, cullingMask(DEFAULT) {
}

void Light::Initialize() {
	type = LIGHT_DIRECTIONAL;
	color = 1.f;
	intensity = 1.f;
	indirectMultiplier = 1.f;
	castShadow = true;
	strength = 1.f;
	cullingMask = DEFAULT;
}

Component * Light::Clone() const {
	return new Light(*this);
}

void Light::SetActive(bool const & state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("LIGHT_ACTIVE", new Events::AnyType<Light*>(this));
} 