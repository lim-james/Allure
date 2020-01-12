#include "Button.h"

#include <Events/EventsManager.h>

Button::Button() {
	isEnabled = true;
	for (auto& h : handlers) {
		h = nullptr;
	}
}

void Button::Initialize() {
	isEnabled = true;
	for (auto& h : handlers) {
		h = nullptr;
	}
}

void Button::SetActive(const bool & state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("BUTTON_ACTIVE", new Events::AnyType<Button*>(this));
}

