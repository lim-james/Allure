#ifndef AI_ATTRIBUTE_H
#define AI_ATTRIBUTE_H

#include "Component.h"

#include <Math/Vectors.hpp>

struct AIAttribute : Component {

	float speed;
	vec3f destination;

	void Initialize() override;

};

#endif
