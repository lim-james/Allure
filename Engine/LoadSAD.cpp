#include "LoadSAD.h"

#include <Logger/Logger.h>

#include <fstream>

std::map<std::string, SpriteAnimationData> Load::SAD(std::string const & filepath) {
	std::map<std::string, SpriteAnimationData> result;

	std::ifstream ifs(filepath);

	if (!ifs.is_open()) {
		Debug::Error << '"' << filepath << "\" not found.\n";
		return result;
	}

	std::string key;
	Keyframe buffer;

	vec2i tilemapSize;
	vec4i cellRect;

	while (!ifs.eof()) {
		ifs >> key >> buffer.duration >> tilemapSize >> cellRect;
		buffer.SetTilemapSize(tilemapSize.x, tilemapSize.y);
		buffer.SetCellRect(cellRect.x, cellRect.y, cellRect.z, cellRect.w);
		result[key].frames.push_back(buffer);
	}

	return result;
}
