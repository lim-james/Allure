#include "LoadTilemap.h"

#include "LoadTexture.h"
#include <Logger/Logger.h>
#include <Loaders/LoadCSV.hpp>	
#include <Helpers/StringHelpers.h>

#include <fstream>

TilemapPalette Load::TMP(std::string const & filepath) {
	TilemapPalette result;

	std::ifstream ifs(filepath);

	if (!ifs.is_open()) {
		Debug::Error << '"' << filepath << "\" not found.\n";
		return result;
	}

	unsigned x, y;
	std::string texturePath;

	while (!ifs.eof()) {
		ifs >> x >> y;
		std::getline(ifs, texturePath);
		if (texturePath == "") continue;

		TilemapTexture texture;
		texture.SetDimensions(vec2u(x, y));
		texture.texture = Load::Texture2D(Helpers::Trim(texturePath));
		result.AddTexture(texture);
		texturePath = "";
	}

	return result;
}

TilemapLayout Load::TML(std::string const & filepath) {
	TilemapLayout result;

	const auto data = Load::CSV<std::string>(filepath);

	vec2f position(0.f);
	for (unsigned y = 0; y < data.size(); ++y, --position.y) {
		auto& row = data[y];

		position.x = 0.f;
		for (unsigned x = 0; x < row.size(); ++x, ++position.x) {
			auto item = Helpers::Split<unsigned>(row[x], ' ');

			if (item.empty()) continue;

			while (result.grids.size() <= item[0]) 
				result.grids.push_back({});
			
			result.grids[item[0]].push_back(Tile{ 
				vec2f(static_cast<float>(item[1]), static_cast<float>(item[2])), 
				position 
			});
		}
	}

	return result;
}
