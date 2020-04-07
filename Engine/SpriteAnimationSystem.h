#ifndef SPRITE_ANIMATION_SYSTEM_H
#define SPRITE_ANIMATION_SYSTEM_H

#include "System.h"
#include "SpriteAnimation.h"

#include <Events/Event.h>

#include <vector>

class SpriteAnimationSystem : public System {

	std::vector<SpriteAnimation*> components;

public:

	~SpriteAnimationSystem() override;

	void Initialize() override;
	void Update(float const& dt) override;

private:

	// Event handlers

	void ActiveHandler(Events::Event* event);

	// helper methods

	void ResetAnimation(SpriteAnimationData& animation);
	void ProcessAnimation(SpriteAnimation* component, SpriteAnimationData& animation);
	void SetProperties(SpriteAnimation* component, SpriteAnimationData const& animation);

};

#endif
