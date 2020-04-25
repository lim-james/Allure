#include "Layout.h"

#include <Events/EventsManager.h>

void Layout::Initialize() {
	constraints.clear();
}

Component * Layout::Clone() const {
	return new Layout(*this);
}

void Layout::SetActive(bool const & state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("LAYOUT_ACTIVE", new Events::AnyType<Layout*>(this));
}

void Layout::AddConstraint(Constraint const & constraint) {
	for (unsigned i = 0; i < constraints.size(); ++i) {
		if (constraint.self > constraints[i].self) continue;
		constraints.insert(constraints.begin() + i, constraint);
		return;
	}
	constraints.push_back(constraint);
}

std::vector<Constraint> const & Layout::GetConstraints() const {
	return constraints;
}
