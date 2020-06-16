#ifndef COLLIDER_SYSTEM_H
#define COLLIDER_SYSTEM_H

#include "System.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "LineCollider.h"
#include "RayCast.h"
#include "Plane.h"

#define BOX_COLLIDER		0
#define SPHERE_COLLIDER		1
#define LINE_COLLIDER		2
#define COLLIDER_TYPE_COUNT 3

class ColliderSystem : public System {

	std::map<unsigned, std::map<unsigned, Handler<void, Collider* const, Collider* const>>> checks;

	std::map<Collider*, std::map<Collider*, bool>> history;
	std::vector<Collider*> colliders[COLLIDER_TYPE_COUNT];

public:

	void Initialize() override;
	void Update(float const& dt) override;
	void FixedUpdate(float const& dt) override;

private:

	// Event handlers
	void BoxActiveHandler(Events::Event* event);
	void SphereActiveHandler(Events::Event* event);
	void LineActiveHandler(Events::Event* event);
	void RayCastHandler(Events::Event* event);

	CollisionData RayBox(Ray const& ray, BoxCollider * const box);

	void HandleCollision(Collider* const a, Collider* const b, bool const& intersecting);
	void PerformAction(unsigned const& action, Collider* const a, Collider* const b);

	void SphereSphere(Collider* const a, Collider* const b);
	void SphereLine(Collider* const a, Collider* const b);

};

#endif
