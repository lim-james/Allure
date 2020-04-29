#include "BoxCollider.h"

#include <Events/EventsManager.h>

void BoxCollider::Initialize() {
	scale = 1.f;
	offset = 0.f;
}

Component * BoxCollider::Clone() const {
	return new BoxCollider(*this);
}

void BoxCollider::SetActive(bool const & state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("BOX_COLLIDER_ACTIVE", new Events::AnyType<BoxCollider*>(this));
}
