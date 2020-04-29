#include "Button.h"

#include <Events/EventsManager.h>

Button::Button() {
	isEnabled = true;
	for (auto& h : handlers) {
		h.UnbindAll();
	}
}

void Button::Initialize() {
	isEnabled = true;
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

