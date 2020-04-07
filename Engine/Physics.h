#ifndef PHYSICS_H
#define PHYSICS_H

#include "Component.h"

#include <Math/Vectors.h>

struct Physics : Component {

	bool useGravity;

	float drag;
	vec3f force;
	vec3f velocity;

	Physics();
	 
	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	void SetMass(float const& _mass);
	float GetMass() const;
	float GetInverseMass() const;

private:

	float mass;
	float inverseMass;

};

#endif
