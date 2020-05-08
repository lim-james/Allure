#include "BoxCollider.h"

#include <Events/EventsManager.h>

Component * BoxCollider::Clone() const {
	return new BoxCollider(*this);
}

void BoxCollider::SetActive(bool const & state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("BOX_COLLIDER_ACTIVE", new Events::AnyType<BoxCollider*>(this));
}
