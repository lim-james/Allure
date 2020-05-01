#include "Light.h"

#include "Layers.h"

#include <Math/Mat4Transform.hpp>
#include <Events/EventsManager.h>

Light::Light() 
	: type(LIGHT_DIRECTIONAL)
	, innerCutOff(12.5f)
	, outerCutOff(17.5f)
	, color(1.f) 
	, intensity(1.f) 
	, castShadows(true)
	, strength(1.f)
	, cullingMask(DEFAULT)
	, shadowMap(0) {
}

void Light::Initialize() {
	type = LIGHT_DIRECTIONAL;
	innerCutOff = 12.5f;
	outerCutOff = 17.5f;
	color = 1.f;
	intensity = 1.f;
	strength = 1.f;
	cullingMask = DEFAULT;
	shadowMap = 0;

	SetCastShadows(true);
}

Component * Light::Clone() const {
	return new Light(*this);
}

void Light::SetActive(bool const & state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("LIGHT_ACTIVE", new Events::AnyType<Light*>(this));
}

mat4f Light::GetProjectionMatrix() const {
	mat4f result;

	if (type == LIGHT_SPOT) {
		result = Math::Perspective(outerCutOff * 2.0f, 1.0f, 0.1f, 100.0f);
	} else if (type == LIGHT_DIRECTIONAL) {
		result = Math::Orthographic(-10.0f, 10.0f, -10.0f, 10.0f, -10.f, 100.0f);
	}

	return result;
}

bool const & Light::CastShadows() const {
	return castShadows;
}

void Light::SetCastShadows(bool const & state) {
	castShadows = state;
	EventsManager::Get()->Trigger("LIGHT_CAST_SHADOWS", new Events::AnyType<Light*>(this));
}
