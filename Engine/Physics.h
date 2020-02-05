#ifndef PHYSICS_H
#define PHYSICS_H

#include "Component.h"

#include <Math/Vectors.hpp>

struct Physics : Component {

	bool useGravity;

	float drag;
	vec3f force;
	vec3f velocity;

	Physics();
	 
	void Initialize() override;
	void SetActive(const bool& state) override;

	void SetMass(const float& _mass);
	float GetMass() const;
	float GetInverseMass() const;

private:

	float mass;
	float inverseMass;

};

#endif
