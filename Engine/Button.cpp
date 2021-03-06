#include "Button.h"

#include <Events/EventsManager.h>

Button::Button() {
	offset = 0.f;
	scale = 1.f;

	isEnabled = true;
	blockRaycast = true;
	for (auto& h : handlers) {
		h.UnbindAll();
	}
}

void Button::Initialize() {
	isEnabled = true;
	blockRaycast = true;
	for (auto& h : handlers) {
		h.UnbindAll();
	}
}

Component * Button::Clone() const {
	return new Button(*this);
}

void Button::SetActive(bool const& state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("BUTTON_ACTIVE", new Events::AnyType<Button*>(this));
}

