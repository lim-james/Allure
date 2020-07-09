#ifndef BEAT_COLLISION_HANDLER_H
#define BEAT_COLLISION_HANDLER_H

#include "Script.h"

#include "StateContainer.h"
#include "Material.h"

struct BeatCollisionHandler : Script {
	using base_type = BeatCollisionHandler;

	float fadeInDuration;
	float fadeOutDuration;

	Material::Base* material;

	void Awake() override;
	virtual void OnHit(unsigned target);

private:

	StateContainer* container;

};

#endif
