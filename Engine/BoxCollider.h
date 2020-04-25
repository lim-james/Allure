#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "Component.h"

#include <Math/Vectors.h>

struct BoxCollider : Component {

	vec3f scale, offset;

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

};

#endif
