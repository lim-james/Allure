#include "Constraint.h"

Constraint::Constraint(short const & self, Transform * const item, short const & other, float multiplier, float constant, Camera * const view)
	: self(self) 
	, item(item)
	, other(other)
	, multiplier(multiplier)
	, constant(constant)
	, view(view) {}
