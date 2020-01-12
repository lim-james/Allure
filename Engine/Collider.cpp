#include "Collider.h"

#include <Events/EventsManager.h>

Collider::Collider() {
	for (auto& h : handlers) {
		h = nullptr;
	}
}

void Collider::Initialize() {
	for (auto& h : handlers) {
		h = nullptr;
	}
}

void Collider::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("COLLIDER_ACTIVE", new Events::AnyType<Collider*>(this));
}

