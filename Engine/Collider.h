#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

#include <Math/Vectors.h>
#include <Bit/BitField.h>
#include <Handler/Handler.h>

#define COLLISION_ENTER 0
#define COLLISION_EXIT	1
#define COLLISION_STAY	2
#define COLLISION_ACTION_COUNT 3

struct Collider : Component {

	bool isTrigger;
	vec3f scale, offset;

	BitField ignoreMask;
	Handler<void, unsigned> handlers[COLLISION_ACTION_COUNT];

	Collider();
	virtual void Initialize();

};

#endif
