#ifndef SPRITE_ANIMATION_DATA_H
#define SPRITE_ANIMATION_DATA_H

#include "Keyframe.h"

#include <vector>
#include <functional>

struct SpriteAnimationData {

	float et;
	unsigned currentFrame;

	std::vector<Keyframe> frames;
	bool loop;

	std::function<void()> completed;

	SpriteAnimationData();

};

#endif
