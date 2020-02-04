#include "Component.h"

Component::Component() 
	: entity(0)
	, flags(ACTIVE_BIT | DYNAMIC_BIT) {}

Component::~Component() {}

bool Component::IsActive() const {
	return flags & ACTIVE_BIT;
}

void Component::SetActive(const bool & _active) {
	flags |= (ACTIVE_BIT & static_cast<unsigned>(_active));
}

bool Component::IsDynamic() const {
	return flags & DYNAMIC_BIT;
}

void Component::SetDynamic(const bool & _active) {
	flags |= (DYNAMIC_BIT & static_cast<unsigned>(_active));
}

