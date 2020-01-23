#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "Component.h"

#include <Math/Vectors.hpp>
#include <functional>

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

	std::function<void()> completion;

	ParticleEmitter();

	void Initialize() override;
	void SetActive(const bool& state) override;

	void Play(const std::function<void()>& _completion = nullptr);

};

#endif
