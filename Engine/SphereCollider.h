#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"

struct SphereCollider : Collider {

	float radius;
	
	SphereCollider();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

};

#endif
