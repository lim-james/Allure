#include "Component.h"

Component::Component() 
	: entity(0)
	, flags(ACTIVE_BIT | DYNAMIC_BIT) {}

Component::~Component() {}

bool Component::IsActive() const {
	return flags & ACTIVE_BIT;
}

void Component::SetActive(bool const& state) {
	flags = (flags & !ACTIVE_BIT) | (ACTIVE_BIT * state);
}

bool Component::IsDynamic() const {
	return flags & DYNAMIC_BIT;
}

void Component::SetDynamic(bool const& state) {
	flags = (flags & !DYNAMIC_BIT) | (DYNAMIC_BIT * state);
}

void Component::Unpack(AENotation const& data) {
	resourceID = data.Get<unsigned>("resource");
	entity = data.Get<unsigned>("entity");
	flags = data.Get<unsigned>("flags");
}

void Component::Pack(AENotation& data) {
	data.Set("resource", resourceID);
	data.Set("entity", entity);
	data.Set("flags", flags);
}

