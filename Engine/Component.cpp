#include "Component.h"

Component::Component() 
	: entity(0)
	, flags(ACTIVE_BIT | DYNAMIC_BIT) {}

Component::~Component() {}

bool Component::IsActive() const {
	return flags & ACTIVE_BIT;
}

void Component::SetActive(const bool & _active) {
	flags = (flags & !ACTIVE_BIT) | (ACTIVE_BIT * _active);
}

bool Component::IsDynamic() const {
	return flags & DYNAMIC_BIT;
}

void Component::SetDynamic(const bool & _active) {
	flags = (flags & !DYNAMIC_BIT) | (DYNAMIC_BIT * _active);
}

void Component::Unpack() {
	entity = data.Get<unsigned>("entity");
	flags = data.Get<unsigned>("flags");
}

void Component::Pack() {
	data.Set("entity", entity);
	data.Set("flags", flags);
}

