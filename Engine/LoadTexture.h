#ifndef LOAD_TEXTURE_H
#define LOAD_TEXTURE_H

#include <map>
#include <string>

namespace Load {

	static std::map<std::string, unsigned> textureCache;
	unsigned Texture(std::string const& filepath);

}

#endif
