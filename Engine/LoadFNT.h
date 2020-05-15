#ifndef LOAD_FNT_H
#define LOAD_FNT_H

#include "Font.h"

#include <map>
#include <string>

namespace Load {

	static std::map<std::string, Font*> fontCache;

	Font* FNT(std::string const& fntPath, std::string const& imagePath);

	void ClearFontCache();

}

#endif
