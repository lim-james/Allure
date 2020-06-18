#ifndef PHYSICS_H
#define PHYSICS_H

#include "Component.h"

#include <Math/Vectors.h>

struct Physics : Component {

	float drag;
	float angularDrag;

	bool useGravity;
	bool interpolate;

	vec3f velocity;
	vec3f angularVelocity;

	Physics();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	float const& GetMass() const;
	float const& GetInverseMass() const;
	void SetMass(float const& value);

	void AddForce(vec3f const& force);

private:

	float mass;
	float invMass;

	vec3f netForce;
	vec3f torque;

	friend class PhysicsSystem;

};

#endif
