#include "MapData.h"

#include <Logger/Logger.h>
#include <fstream>

MapData Load::Map(std::string const & filepath) {
	MapData result;

	std::ifstream ifs(filepath);
	if (!ifs.is_open()) {
		Debug::Warn << "Unabled to read to \"" << filepath << "\"\n";
		return result;
	}

	unsigned n, gn;

	std::getline(ifs, result.audioPath);
	ifs >> result.bpm >> n;

	for (unsigned i = 0; i < n; ++i) {
		SpawnGroup group;
		ifs >> group.beatStride >> gn;

		for (unsigned j = 0; j < gn; ++j) {
			unsigned index;
			int count;
			ifs >> index >> count;

			for (; count >= 0; --count) {
				vec3f position; 
				ifs >> position;
				group.enemies[index].push_back(position);
			}
		}
	}

	return result;
}

void Write::Map(std::string const & filepath, MapData const & mapData) {
	std::ofstream ofs(filepath);

	if (!ofs.is_open()) {
		Debug::Warn << "Unabled to write to \"" << filepath << "\"\n";
		return;
	}

	SpawnSchedule schedule = mapData.schedule;
	const unsigned size = schedule.size();
	
	ofs << mapData.audioPath << '\n';
	ofs << mapData.bpm << ' ' << size << '\n';

	for (unsigned i = 0; i < size; ++i) {
		SpawnGroup& group = schedule[i];
		ofs << group.beatStride << ' ' << group.enemies.size() << '\n';

		for (auto& enemy: group.enemies) {
			ofs << enemy.first << ' ' << enemy.second.size() << '\n';
			for (auto& position : enemy.second) 
				ofs << position << ' ';
			ofs << '\n';
		}
	}
}

