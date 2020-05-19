#ifndef LOAD_SAD_H
#define LOAD_SAD_H

#include "SpriteAnimationData.h"

#include <map>
#include <string>

namespace Load {
	std::map<std::string, SpriteAnimationData> SAD(std::string const& filepath);
}

#endif
