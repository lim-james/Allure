#ifndef LAYOUT_H
#define LAYOUT_H

#include "Component.h"
#include "Constraint.h"

#include <vector>

struct Layout : Component {

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	void AddConstraint(Constraint const& constraint);
	Constraint* GetConstraint(short const& type);
	std::vector<Constraint> const& GetConstraints() const;

private:

	std::vector<Constraint> constraints;

};

#endif
