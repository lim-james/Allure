#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "Component.h"

#include <Math/Vectors.h>
#include <Handler/Handler.h>

struct ParticleEmitter : Component {

	float age;
	float duration;

	float spawnInterval;
	float spawnTimer;
	unsigned burstAmount;

	bool loop;
	float lifetime;
	float lifetimeRange;

	vec3f angle;
	vec3f angleRange;

	float speed;
	float speedRange;

	vec3f offset;
	vec3f positionRange;

	vec3f gravity;

	// acceleration towards center
	float accelRad;
	float accelRadRange;

	vec3f startSize;
	vec3f startSizeRange;

	vec3f endSize;
	vec3f endSizeRange;

	unsigned texture;

	vec4f startColor;
	vec4f startColorRange;

	vec4f endColor;
	vec4f endColorRange;

	Handler<void, void> completion;

	ParticleEmitter();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	void Play();

};

#endif
