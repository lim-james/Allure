#ifndef LOAD_TILEMAP_H
#define LOAD_TILEMAP_H

#include "TilemapResource.h"

namespace Load {
	TilemapPalette TMP(std::string const& filepath);
	TilemapLayout TML(std::string const& filepath);
}

namespace Write {
	void TML(TilemapLayout const& layout, std::string const& filepath);
}

#endif
