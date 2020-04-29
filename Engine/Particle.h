#ifndef PARTICLE_H
#define PARTICLE_H

#include "Component.h"

#include <Math/Vectors.h>

struct Particle : Component {

	float age;
	float lifetime;

	vec3f velocity;
	vec3f angularVelocity;

	vec3f startSize;
	vec3f endSize;

	vec4f startColor;
	vec4f endColor;

	Particle();

	void Initialize() override;
	Component* Clone() const override;

};

#endif