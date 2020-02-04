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
	void Update(const float& dt) override;

private:

	// Event handlers

	void ActiveHandler(Events::Event* event);

	// helper methods

	void ResetAnimation(SpriteAnimationData& animation);
	void ProcessAnimation(SpriteAnimation* const component, SpriteAnimationData& animation);
	void SetProperties(SpriteAnimation* const component, SpriteAnimationData& animation);

};

#endif
