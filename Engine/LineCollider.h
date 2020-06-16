#ifndef LINE_COLLIDER_H
#define LINE_COLLIDER_H

#include "Collider.h"

#include <Math/Vectors.h>

struct LineCollider : Collider {
	
	float length;
	vec3f direction;

	LineCollider();
	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

};

#endif
