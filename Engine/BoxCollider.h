#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "Collider.h"

struct BoxCollider : Collider {

	Component* Clone() const override;
	void SetActive(bool const& state) override;

};

#endif
