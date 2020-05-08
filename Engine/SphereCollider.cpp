#include "SphereCollider.h"

#include <Events/EventsManager.h>

Component * SphereCollider::Clone() const {
	return new SphereCollider(*this);
}

void SphereCollider::SetActive(bool const & state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("SPHERE_COLLIDER_ACTIVE", new Events::AnyType<SphereCollider*>(this));
}
