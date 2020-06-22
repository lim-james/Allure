#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "Collider.h"

struct BoxCollider : Collider {
	
	using base_type = BoxCollider;

	Component* Clone() const override;
	void SetActive(bool const& state) override;

};

#endif
