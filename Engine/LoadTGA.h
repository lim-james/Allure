#ifndef LOAD_TGA_H
#define LOAD_TGA_H

#include <map>
#include <string>

namespace Load {

	static std::map<std::string, unsigned> textureCache;

	unsigned TGA(const std::string& filepath);

}

#endif
