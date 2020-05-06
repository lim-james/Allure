#ifndef PHYSICS_H
#define PHYSICS_H

#include "Component.h"

#include <Math/Vectors.h>

struct Physics : Component {

	float drag;
	float angularDrag;

	bool useGravity;

	Physics();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	float const& GetMass() const;
	float const& GetInverseMass() const;
	void SetMass(float const& value);

private:

	float mass;
	float invMass;

	float speed;
	vec3f velocity;
	vec3f angularVelocity;

	vec3f acceleration;
	vec3f angularAcceleration;

	friend class PhysicsSystem;

};

#endif
