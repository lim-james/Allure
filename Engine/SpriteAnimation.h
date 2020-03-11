#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

#include "Component.h"

#include "SpriteAnimationData.h"

#include <map>
#include <string>

struct SpriteAnimation : Component {

	std::map<std::string, SpriteAnimationData> animations;
	std::string currentAnimation, queued;

	SpriteAnimation();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(const bool& state) override;

};

#endif
