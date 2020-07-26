#ifndef SPRITE_ANIMATION_DATA_H
#define SPRITE_ANIMATION_DATA_H

#include "Keyframe.h"

#include <Handler/Handler.h>
#include <vector>
#include <map>
#include <string>

struct SpriteAnimationData {

	float et;
	unsigned currentFrame;

	std::vector<Keyframe> frames;
	bool loop;

	Handler<void, void> completed;

	SpriteAnimationData();

};

typedef std::map<std::string, SpriteAnimationData> SADMap;

#endif
