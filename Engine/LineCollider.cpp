#include "LineCollider.h"

#include <Events/EventsManager.h>

LineCollider::LineCollider() 
	: length(1.f)
	, direction(1.f, 0.f, 0.f) {}

void LineCollider::Initialize() {
	Collider::Initialize();
	length = 1.f;
	direction = vec3f(1.f, 0.f, 0.f);
}

Component * LineCollider::Clone() const {
	return new LineCollider(*this);
}

void LineCollider::SetActive(bool const & state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("LINE_COLLIDER_ACTIVE", new Events::AnyType<LineCollider*>(this));
}
