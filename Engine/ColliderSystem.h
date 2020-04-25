#ifndef COLLIDER_SYSTEM_H
#define COLLIDER_SYSTEM_H

#include "System.h"

#include "BoxCollider.h"
#include "RayCast.h"
#include "Plane.h"

class ColliderSystem : public System {

	std::vector<BoxCollider*> components;

public:

	void Initialize() override;
	void Update(float const& dt) override;

private:

	// Event handlers
	void ActiveHandler(Events::Event* event);
	void RayCastHandler(Events::Event* event);

	CollisionData RayBox(Ray const& ray, BoxCollider * const box);

};

#endif
