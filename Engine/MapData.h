#ifndef MAP_DATA_H
#define MAP_DATA_H

#include "SpawnGroup.h"

struct MapData {
	std::string audioPath;
	unsigned bpm;
	SpawnSchedule schedule;
};

namespace Load {
	MapData Map(std::string const& filepath);
}

namespace Write {
	void Map(std::string const& filepath, MapData const& mapData);
}

#endif
