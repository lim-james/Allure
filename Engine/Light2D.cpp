#include "Light2D.h"

#include <Events/EventsManager.h>

Component * Light2D::Clone() const {
	return new Light2D(*this);
}

void Light2D::SetActive(bool const & state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("LIGHT_2D_ACTIVE", new Events::AnyType<Light2D*>(this));
}

bool const & Light2D::CastShadows() const {
	return castShadows;
}

void Light2D::SetCastShadows(bool const & state) {
	castShadows = state;
	EventsManager::Get()->Trigger("LIGHT_2D_CAST_SHADOWS", new Events::AnyType<Light*>(this));
}

mat4f Light2D::GetProjectionMatrix() const {
	return mat4f();
}
