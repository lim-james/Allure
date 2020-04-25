#ifndef RAY_CAST_H
#define RAY_CAST_H

#include "CollisionData.h"

#include <Events/Event.h>
#include <Math/Vectors.h>
#include <Bit/BitField.h>

struct Ray {
	vec3f origin, direction;

	Ray(vec3f const& origin, vec3f const& direction)
		: origin(origin)
		, direction(direction) {}
};

namespace Events {
	struct RayCast : Event {
		const Ray ray;
		const BitField collisionMask;

		CollisionData * const result;

		RayCast(vec3f const& origin, vec3f const& direction, BitField const& collisionMask, CollisionData * const result) 
			: ray(origin, direction)
			, collisionMask(collisionMask) 
			, result(result) {}
	};
}

#endif
