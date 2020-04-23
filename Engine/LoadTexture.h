#ifndef LOAD_TEXTURE_H
#define LOAD_TEXTURE_H

#include <map>
#include <vector>
#include <string>

namespace Load {

	static std::map<std::string, unsigned> textureCache;

	unsigned Texture2D(std::string const& filepath);
	unsigned Cubemap(std::vector<std::string> const& filepaths);
	unsigned TGA(std::string const& filepath);

}

#endif
