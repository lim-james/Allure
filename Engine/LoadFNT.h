#ifndef LOAD_FNT_H
#define LOAD_FNT_H

#include "Font.h"

#include <map>
#include <string>

namespace Load {

	static std::map<std::string, Font*> fontCache;

	Font* FNT(const std::string& fntPath, const std::string& tgaPath);

	void ClearFontCache();

}

#endif
