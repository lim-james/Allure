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

void Write::TML(TilemapLayout const & layout, std::string const & filepath) {
	std::vector<std::vector<std::string>> data;

	vec2i min = 0;
	vec2i max = 0;

	for (auto& grid : layout.grids) {
		for (auto& tile : grid) {
			const int x = static_cast<int>(tile.position.x);
			const int y = static_cast<int>(tile.position.y);

			if (x < min.x) min.x = x;
			else if (x > max.x) max.x = x;

			if (y < min.y) min.y = y;
			else if (y > max.y) max.y = y;
		}
	}

	for (int i = 0; i <= max.y - min.y; ++i) {
		data.push_back({});
		for (int j = 0; j <= max.x - min.x; ++j) {
			data[i].push_back("");
		}
	}

	for (unsigned i = 0; i < layout.grids.size(); ++i) {
		auto& grid = layout.grids[i];
		const std::string id = std::to_string(i) + ' ';

		for (auto& tile : grid) {
			const int x = static_cast<int>(tile.position.x) - min.x;
			const int y = static_cast<int>(tile.position.y) - min.y;

			const int u = static_cast<int>(tile.uvOffset.x);
			const int v = static_cast<int>(tile.uvOffset.y);

			data[y][x] = id + std::to_string(u) + ' ' + std::to_string(v);
		}
	}

	std::ofstream ofs(filepath);
	for (auto& row : data) {
		for (auto& item : row)
			ofs << item << ',';

		ofs << '\n';
	}

	ofs.close();
}
